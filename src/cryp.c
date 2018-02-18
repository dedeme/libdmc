// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"
#include <time.h>

static char *b64_base =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *cryp_genk (int lg) {
  if (lg <= 0)
    THROW
      exc_illegal_argument("lg", "> 0", str_printf("%d", lg))
    _THROW

  uint len = strlen(b64_base);
  char *r = ATOMIC(lg + 1);
  char *p = r + lg;
  *p-- = 0;
  while (lg--) {
    *p-- = *(b64_base + rnd_i(len));
  }
  return r;
}

char *cryp_key (char *key, int lg) {
  if (!key) THROW exc_null_pointer("key") _THROW
  if (!*key)
    THROW
      exc_illegal_argument("key", "not blank string", "blank string")
    _THROW

  char *k0 = str_printf(
    "%scodified in irreversibleDeme is good, very good!\n\r8@@", key);
  char *b64 = b64_encode(k0);
  Bytes *k = b64_decode_bytes(b64);

  unsigned char *ka = bytes_bs(k);
  size_t lenk = bytes_length(k);
  unsigned char sum = 0;
  unsigned char *p = ka;
  REPEAT(lenk) {
    sum += *p++;
  }_REPEAT

  size_t lg2 = lg + lenk;
  Bytes *rbs = bytes_new_len(lg2);
  unsigned char *ra = bytes_bs(rbs);
  Bytes *rbs1 = bytes_new_len(lg2);
  unsigned char *ra1 = bytes_bs(rbs1);
  Bytes *rbs2 = bytes_new_len(lg2);
  unsigned char *ra2 = bytes_bs(rbs2);

  size_t ik = 0;
  size_t v1, v2, v3, v4;
  for (size_t i = 0; i < lg2; ++i) {
    v1 = ka[ik];
    v2 = v1 + ka[v1 % lenk];
    v3 = v2 + ka[v2 % lenk];
    v4 = v3 + ka[v3 % lenk];
    sum = sum + i + v4;
    ra1[i] = sum;
    ra2[i] = sum;
    ++ik;
    if (ik == lenk) {
      ik = 0;
    }
  }

  for (size_t i = 0; i < lg2; ++i) {
    v1 = ra2[i];
    v2 = v1 + ra2[v1 % lg2];
    v3 = v2 + ra2[v2 % lg2];
    v4 = v3 + ra2[v3 % lg2];
    sum = sum + v4;
    ra2[i] = sum;
    ra[i] = sum + ra1[i];
  }

  return str_sub(b64_encode_bytes(rbs), 0, lg);
}

char *cryp_cryp (char *k, char *s) {
  if (!k) THROW exc_null_pointer("k") _THROW
  if (!s) THROW exc_null_pointer("s") _THROW
  if (!*k)
    THROW
      exc_illegal_argument("k", "not blank string", "blank string")
    _THROW

  char *b64 =b64_encode(s);

  size_t lg = strlen(b64);
  char *k2 = cryp_key(k, lg);

  Bytes *rbs = bytes_new_len(lg);

  unsigned char *prbs = bytes_bs(rbs);
  unsigned char *pk2 = (unsigned char *)k2;
  unsigned char *pb64 = (unsigned char *)b64;
  REPEAT(lg) {
    *prbs++ = (*pk2++) + (*pb64++);
  }_REPEAT

  return b64_encode_bytes(rbs);
}

char *cryp_decryp (char *k, char *c) {
  if (!k) THROW exc_null_pointer("k") _THROW
  if (!c) THROW exc_null_pointer("c") _THROW
  if (!*k)
    THROW
      exc_illegal_argument("k", "not blank string", "blank string")
    _THROW

  Bytes *bs = b64_decode_bytes(c);

  size_t lg = bytes_length(bs);
  char *k2 = cryp_key(k, lg);

  char *b64 = ATOMIC(lg + 1);

  unsigned char *pbs = bytes_bs(bs);
  unsigned char *pk2 = (unsigned char *)k2;
  char *pb64 = b64;
  REPEAT(lg) {
    *pb64++ = *pbs++ - *pk2++;
  }_REPEAT
  *pb64 = 0;

  return b64_decode(b64);
}

char *cryp_auto_cryp (int nK, char *s) {
  nK = nK < 1 ? 0 : nK > 64 ? 63 : nK - 1;
  char *k = cryp_genk(nK + 1);
  return str_printf("%c%s%s", b64_base[nK], k, cryp_cryp(k, s));
}

char *cryp_auto_decryp (char *b64) {
  int nK = str_cindex(b64_base, *b64) + 1;
  char *key = str_sub(b64, 1, nK + 1);
  return cryp_decryp(key, str_sub_end(b64, nK + 1));
}

inline
char *cryp_encode (char *k, int nK, char *s) {
  return cryp_cryp(k, cryp_auto_cryp(nK, s));
}

inline
char *cryp_decode (char *k, char *b41) {
  return cryp_auto_decryp(cryp_decryp(k, b41));
}

