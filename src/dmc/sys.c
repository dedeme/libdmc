// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/sys.h"
#include <locale.h>

void sys_locale (char *language) {
  setlocale (LC_ALL, language);
}
