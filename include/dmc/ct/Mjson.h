// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Map of Json *

#ifndef DM_MJSON_H
  #define DM_MJSON_H

typedef struct json_Json Json;

#define TY Json
#define FN json
#include "dmc/tpl/tmap.h"
#undef TY
#undef FN

#endif
