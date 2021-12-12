// Copyright 22-Nov-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/cryp.h"
#include <stdio.h>
#include <stdlib.h>
#include "dmc/err.h"
#include "dmc/b64.h"
#include "dmc/str.h"
#include "dmc/Buf.h"
#include "dmc/rnd.h"

static char *b64_base =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *cryp_genk (int lg) {
  if (lg <= 0) FAIL(str_f("lg (%d) is less than 0"));

  int len = strlen(b64_base);
  char *r = ATOMIC(lg + 1);
  char *p = r + lg;
  *p-- = 0;
  while (lg--) {
    *p-- = *(b64_base + rnd_i(len));
  }
  return r;
}

char *cryp_key (char *key, int lg) {
  if (!*key) FAIL("key == \"\"");

  char *k0 = str_f(
    "%scodified in irreversibleDeme is good, very good!\n\r8@@", key);
  char *b64 = b64_encode(k0);
  Bytes *k = b64_decode_bytes(b64);

  unsigned char *ka = (k->bs);
  size_t lenk = k->len;
  unsigned char sum = 0;
  unsigned char *p = ka;
  for (int i = 0; i < lenk; ++i) sum += *p++;

  size_t lg2 = lg + lenk;
  Bytes *rbs = bytes_bf_new(lg2);
  unsigned char *ra = rbs->bs;
  Bytes *rbs1 = bytes_bf_new(lg2);
  unsigned char *ra1 = rbs1->bs;
  Bytes *rbs2 = bytes_bf_new(lg2);
  unsigned char *ra2 = rbs2->bs;

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

  return str_left(b64_encode_bytes(rbs), lg);
}

char *cryp_cryp (char *k, char *s) {
  if (!*k) FAIL("k == \"\"");

  char *b64 = b64_encode(s);

  size_t lg = strlen(b64);
  char *k2 = cryp_key(k, lg);

  Bytes *rbs = bytes_bf_new(lg);

  unsigned char *prbs = rbs->bs;
  unsigned char *pk2 = (unsigned char *)k2;
  unsigned char *pb64 = (unsigned char *)b64;
  for(int i = 0; i < lg; ++i) *prbs++ = (*pk2++) + (*pb64++);

  return b64_encode_bytes(rbs);
}

char *cryp_decryp (char *k, char *c) {
  if (!*k) FAIL("k == \"\"");

  Bytes *bs = b64_decode_bytes(c);

  size_t lg = bs->len;
  char *k2 = cryp_key(k, lg);

  char *b64 = ATOMIC(lg + 1);

  unsigned char *pbs = bs->bs;
  unsigned char *pk2 = (unsigned char *)k2;
  char *pb64 = b64;
  for(int i = 0; i < lg; ++i) *pb64++ = *pbs++ - *pk2++;
  *pb64 = 0;

  return b64_decode(b64);
}

char *cryp_auto_cryp (int nk, char *s) {
  nk = nk < 1 ? 0 : nk > 64 ? 63 : nk - 1;
  char *n = str_c(b64_base[nk]);
  char *k = cryp_genk(nk + 1);
  s = cryp_cryp(k, s);
  return str_cat(n, k, s, NULL);
}

char *cryp_auto_decryp (char *b64) {
  char *s = b64;
  int nk = str_cindex(b64_base, *s) + 1;
  Buf *bf = buf_new();
  buf_add_buf(bf, s + 1, nk);
  char *key = buf_to_str(bf);
  return cryp_decryp(key, str_right(b64, nk + 1));
}

char *cryp_encode (char *k, int nk, char *s) {
  return cryp_cryp(k, cryp_auto_cryp(nk, s));
}

char *cryp_decode (char *k, char *b64) {
  return cryp_auto_decryp(cryp_decryp(k, b64));
}
