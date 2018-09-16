// Copyright 16-Sep-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Kv of Json *

#ifndef DM_CT_KJSON_H
  #define DM_CT_KJSON_H

typedef struct json_Json Json;

#define TY Json
#define FN json
#include "dmc/tpl/tkv.h"
#undef TY
#undef FN

#endif
