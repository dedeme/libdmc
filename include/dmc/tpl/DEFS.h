// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DMC_TPL_DEFS
  #define DMC_TMP_DEFS

///
#define TPL_CAT(a, b) TPL_CAT_EXPANSION(a, b)
///
#define TPL_CAT_EXPANSION(a, b) a ## b

///
#define TPL_CAT_FUN(a, b, c) TPL_CAT_FUN_EXPANSION(a, b, c)
///
#define TPL_CAT_FUN_EXPANSION(a, b, c) a ## b ## _ ## c

#endif
