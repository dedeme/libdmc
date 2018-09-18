// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "dmc/Date.h"
#include "dmc/str.h"
#include "dmc/Dec.h"
#include "dmc/ct/Achar.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

Date date_new(int day, int month, int year) {
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = 12;

  return (Date) mktime(&t);
}

inline
Date date_now() {
  return time(NULL);
}

Date date_from_str(char *date) {
  XNULL(date)

  char tpl[5];
  memset (tpl, 0, 5);
  memcpy(tpl, date + 6, 2);
  int d = atoi(tpl);
  memcpy(tpl, date + 4, 2);
  int m = atoi(tpl);
  memcpy(tpl, date, 4);
  int y = atoi(tpl);
  return date_new(d, m, y);
}

Date date_from_iso(char *date) {
  return date_from_iso_sep(date, '/');
}

Date date_from_us(char *date) {
  return date_from_us_sep(date, '/');
}

Date _date_from_sep (char *d, char *m, char *y) {
  if (str_len(d) > 2 || !dec_digits(d)) return 0;
  if (str_len(m) > 2 || !dec_digits(m)) return 0;
  if (str_len(y) > 4 || !dec_digits(y)) return 0;
  int year = atoi(y);
  if (year < 100) {
    year += 2000;
  }
  return date_new(atoi(d), atoi(m), year);
}

Date date_from_iso_sep (char *date, char sep) {
  XNULL(date)
  Achar *parts = str_csplit(date, sep);
  if (achar_size(parts) != 3) return 0;
  return _date_from_sep(
    achar_get(parts, 0), achar_get(parts, 1), achar_get(parts, 2)
  );
}

Date date_from_us_sep (char *date, char sep) {
  XNULL(date)
  Achar *parts = str_csplit(date, sep);
  if (achar_size(parts) != 3) return 0;
  return _date_from_sep(
    achar_get(parts, 1), achar_get(parts, 0), achar_get(parts, 2)
  );
}

inline
bool date_eq(Date t1, Date t2) {
  return t1 == t2;
}

inline
int date_cmp(Date t1, Date t2) {
  return t1 - t2;
}

int date_df(Date t1, Date t2) {
  double tmp = difftime(t1, t2) / 86400;
  return tmp >= 0 ? tmp + 0.5 : tmp - 0.5;
}

inline
Date date_add(Date this, int days) {
  return this + days * 86400;
}

inline
int date_day(Date this) {
  return localtime(&this)->tm_mday;
}

inline
int date_month(Date this) {
  return localtime(&this)->tm_mon + 1;
}

inline
int date_year(Date this) {
  return localtime(&this)->tm_year + 1900;
}

char *date_format(Date this, char *template) {
  XNULL(template)

  char *s, *rs;
  struct tm *t = localtime(&this);
  int size = 126;
  while (1) {
    rs = (char *)calloc(size, 1);
    if (strftime (rs, size, template, t)) {
      s = str_copy(rs);
      free(rs);
      break;
    }
    free(rs);
    size += size;
  }
  return s;
}

inline
char *date_to_str(Date this) {
  return date_format(this, "%Y%m%d");
}

inline
char *date_to_iso(Date this) {
  return date_format(this, "%d/%m/%Y");
}

inline
char *date_to_us(Date this) {
  return date_format(this, "%m/%d/%Y");
}
