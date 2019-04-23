// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "date_tests.h"
#include <assert.h>
#include "dmc/date.h"

void date_tests(void) {
  puts("Date tests");

  time_t d = date_now();
  char *s = NULL;
  s = date_to_str(d);
  assert(8 == strlen(s));
  d = date_new(2, 4, 2010);
  s = date_to_str(d);
  assert(str_eq(s, "20100402"));
  s = date_to_iso(d);
  assert(str_eq(s, "02/04/2010"));
  d = date_from_str("19881231");
  s = date_to_str(d);
  assert(str_eq(s, "19881231"));
  s = date_to_us(d);
  assert(str_eq(s, "12/31/1988"));
  d = date_from_iso("01/02/2020");
  s = date_to_str(d);
  assert(str_eq(s, "20200201"));
  d = date_from_us("02/01/2020");
  s = date_to_str(d);
  assert(str_eq(s, "20200201"));
  time_t d1 = date_new(29, 2, 2013);
  time_t d2 = date_new(6, 3, 2013);
  time_t d3 = date_new(30, 4, 2013);

  assert(-5 == date_df(d1, d2));
  assert(date_cmp(d1, d2) < 0);
  assert(55 == date_df(d3, d2));
  assert(date_cmp(d3, d2) > 0);
  assert(date_eq(d1, date_add(d2, -5)));
  assert(!date_cmp(d1, date_add(d2, -5)));
  assert(date_eq(d3, date_add(d2, 55)));

  assert(date_day(d1) == 1 && date_day(d2) == 6 &&date_day(d3) == 30);
  assert(date_month(d1) == 3 && date_month(d2) == 3 &&date_month(d3) == 4);
  assert(date_year(d1) == 2013);

  assert(!date_from_iso_sep("", '/'));
  assert(!date_from_iso_sep("123/34/34", '/'));
  assert(!date_from_iso_sep("23/34", '/'));
  assert(!date_from_iso_sep("23/34/23/34", '/'));
  assert(!date_from_iso_sep("23/34/234b", '/'));
  assert(!date_from_iso_sep("23c/34/234b", '/'));
  assert(!date_from_iso_sep("23/34x/234", '/'));

  assert(!date_from_us_sep("", '/'));
  assert(!date_from_us_sep("123/34/34", '/'));
  assert(!date_from_us_sep("23/34", '/'));
  assert(!date_from_us_sep("23/34/23/34", '/'));
  assert(!date_from_us_sep("23/34/234b", '/'));
  assert(!date_from_us_sep("23c/34/234b", '/'));
  assert(!date_from_us_sep("23/34x/234", '/'));

  assert(date_eq(date_from_iso("01/02/2015"),
    date_from_iso_sep("1/2/2015", '/')));

  assert(date_eq(date_from_iso("01/02/15"),
    date_from_iso_sep("1/02/2015", '/')));

  assert(date_eq(date_from_us("02/01/2015"),
    date_from_us_sep("2/1/2015", '/')));

  assert(date_eq(date_from_us("02/01/2015"),
    date_from_us_sep("02/1/15", '/')));

  puts("    Finished");
}

