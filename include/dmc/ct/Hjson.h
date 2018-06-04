// Copyright 04-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hash of Json *

#ifndef DM_CT_HJSON_H
  #define DM_CT_HJSON_H

typedef struct json_Json Json;

#define TY Json
#define FN json
#include "dmc/tpl/thash.h"
#undef TY
#undef FN

#endif
