// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/cryp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmc/std.h"
#include "dmc/b64.h"
#include "dmc/rnd.h"

static char *b64_base =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *cryp_genk (Gc *gc, int lg) {
  if (lg <= 0)
    EXC_ILLEGAL_ARGUMENT("lg", "> 0", str_f(gc_new(), "%d", lg))

  int len = strlen(b64_base);
  char *r = gc_add(gc, malloc(lg + 1));
  char *p = r + lg;
  *p-- = 0;
  while (lg--) {
    *p-- = *(b64_base + rnd_i(len));
  }
  return r;
}

char *cryp_key (Gc *gc, char *key, int lg) {
  if (!*key)
    EXC_ILLEGAL_ARGUMENT("key", "No blank", "blank")

  Gc *gcl = gc_new();
  char *k0 = str_f(gcl,
    "%scodified in irreversibleDeme is good, very good!\n\r8@@", key);
  char *b64 = b64_encode(gcl, k0);
  Bytes *k = b64_decode_bytes(gcl, b64);

  unsigned char *ka = bytes_bs(k);
  size_t lenk = bytes_len(k);
  unsigned char sum = 0;
  unsigned char *p = ka;
  REPEAT(lenk) {
    sum += *p++;
  }_REPEAT

  size_t lg2 = lg + lenk;
  Bytes *rbs = bytes_new_bf(gcl, lg2);
  unsigned char *ra = bytes_bs(rbs);
  Bytes *rbs1 = bytes_new_bf(gcl, lg2);
  unsigned char *ra1 = bytes_bs(rbs1);
  Bytes *rbs2 = bytes_new_bf(gcl, lg2);
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

  char *r = str_left(gc, b64_encode_bytes(gcl, rbs), lg);
  gc_free(gcl);
  return r;
}

char *cryp_cryp (Gc *gc, char *s, char *k) {
  if (!*k)
    EXC_ILLEGAL_ARGUMENT("k", "No blank", "blank")

  Gc *gcl = gc_new();
  char *b64 = b64_encode(gcl, s);

  size_t lg = strlen(b64);
  char *k2 = cryp_key(gcl, k, lg);

  Bytes *rbs = bytes_new_bf(gcl, lg);

  unsigned char *prbs = bytes_bs(rbs);
  unsigned char *pk2 = (unsigned char *)k2;
  unsigned char *pb64 = (unsigned char *)b64;
  REPEAT(lg) {
    *prbs++ = (*pk2++) + (*pb64++);
  }_REPEAT

  char *r = b64_encode_bytes(gc, rbs);
  gc_free(gcl);
  return r;
}

char *cryp_decryp (Gc *gc, char *c, char *k) {
  if (!*k)
    EXC_ILLEGAL_ARGUMENT("k", "No blank", "blank")

  Gc *gcl = gc_new();
  Bytes *bs = b64_decode_bytes(gcl, c);

  size_t lg = bytes_len(bs);
  char *k2 = cryp_key(gcl, k, lg);

  char *b64 = gc_add(gcl, malloc(lg + 1));

  unsigned char *pbs = bytes_bs(bs);
  unsigned char *pk2 = (unsigned char *)k2;
  char *pb64 = b64;
  REPEAT(lg) {
    *pb64++ = *pbs++ - *pk2++;
  }_REPEAT
  *pb64 = 0;

  char *r = b64_decode(gc, b64);
  gc_free(gcl);
  return r;
}

char *cryp_auto_cryp (Gc *gc, char *s, int nk) {
  Gc *gcl = gc_new();
  nk = nk < 1 ? 0 : nk > 64 ? 63 : nk - 1;
  char *n = str_new_c(gcl, b64_base[nk]);
  char *k = cryp_genk(gcl, nk + 1);
  s = cryp_cryp(gcl, s, k);
  char *r = str_cat(gc, n, k, s, NULL);
  gc_free(gcl);
  return r;
}

char *cryp_auto_decryp (Gc *gc, char *b64) {
  Gc *gcl = gc_new();
  char *s = b64;
  int nk = str_cindex(b64_base, *s) + 1;
  Buf *bf = buf_new(gcl);
  buf_add_buf(bf, s + 1, nk);
  char *key = buf_to_str(gcl, bf);
  char *r = cryp_decryp(gc, str_right(gcl, b64, nk + 1), key);
  gc_free(gcl);
  return r;
}

char *cryp_encode (Gc *gc, char *s, int nk, char *k) {
  Gc *gcl = gc_new();
  char *r = cryp_cryp(gc, cryp_auto_cryp(gcl, s, nk), k);
  gc_free(gcl);
  return r;
}

char *cryp_decode (Gc *gc, char *b64, char *k) {
  Gc *gcl = gc_new();
  char *r = cryp_auto_decryp(gc, cryp_decryp(gcl, b64, k));
  gc_free(gcl);
  return r;
}

