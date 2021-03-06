// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/date.h"
#include "dmc/Dec.h"
#include <sys/time.h>

time_t date_new(int day, int month, int year) {
  struct tm t;
  memset(&t, 0, sizeof(struct tm));
  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = 12;

  return (time_t) mktime(&t);
}

inline
time_t date_now() {
  return time(NULL);
}

time_t date_from_str(char *date) {
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

time_t date_from_iso(char *date) {
  return date_from_iso_sep(date, '/');
}

time_t date_from_us(char *date) {
  return date_from_us_sep(date, '/');
}

static time_t _date_from_sep (char *d, char *m, char *y) {
  if (strlen(d) > 2 || !dec_digits(d)) return 0;
  if (strlen(m) > 2 || !dec_digits(m)) return 0;
  if (strlen(y) > 4 || !dec_digits(y)) return 0;
  int year = atoi(y);
  if (year < 100) {
    year += 2000;
  }
  return date_new(atoi(d), atoi(m), year);
}

time_t date_from_iso_sep (char *date, char sep) {
  // Arr[char]
  Arr *parts = str_csplit(date, sep);
  if (arr_size(parts) != 3) {
    return 0;
  }
  time_t r = _date_from_sep(
    arr_get(parts, 0), arr_get(parts, 1), arr_get(parts, 2)
  );
  return r;
}

time_t date_from_us_sep (char *date, char sep) {
  // Arr[char]
  Arr *parts = str_csplit(date, sep);
  if (arr_size(parts) != 3) {
    return 0;
  }
  time_t r = _date_from_sep(
    arr_get(parts, 1), arr_get(parts, 0), arr_get(parts, 2)
  );
  return r;
}

int date_eq(time_t t1, time_t t2) {
  return t1 == t2;
}

int date_cmp(time_t t1, time_t t2) {
  return t1 - t2;
}

int date_df(time_t t1, time_t t2) {
  double tmp = difftime(t1, t2) / 86400;
  return tmp >= 0 ? tmp + 0.5 : tmp - 0.5;
}

time_t date_add(time_t this, int days) {
  return this + days * 86400;
}

int date_day(time_t this) {
  return localtime(&this)->tm_mday;
}

int date_month(time_t this) {
  return localtime(&this)->tm_mon + 1;
}

int date_year(time_t this) {
  return localtime(&this)->tm_year + 1900;
}

char *date_f(time_t this, char *template) {
  char *s, *rs;
  struct tm *t = localtime(&this);
  int size = 126;
  while (1) {
    rs = (char *)calloc(size, 1); // calloc ok
    if (strftime (rs, size, template, t)) {
      s = str_new(rs);
      free(rs); // free ok
      break;
    }
    free(rs); // free ok
    size += size;
  }
  return s;
}

char *date_to_str(time_t this) {
  return date_f(this, "%Y%m%d");
}

char *date_to_iso(time_t this) {
  return date_f(this, "%d/%m/%Y");
}

char *date_to_us(time_t this) {
  return date_f(this, "%m/%d/%Y");
}

Js *date_to_js(time_t this) {
  return (Js *)js_wl(this);
}

time_t date_from_js(Js *js) {
  return js_rl(js);
}

DateTm *date_tm_now () {
  DateTm *r = MALLOC(DateTm);
  gettimeofday (r, NULL);
  return r;
}

DateTm *date_tm_tdf (DateTm *t1, DateTm *t2) {
  DateTm *r = MALLOC(DateTm);
  r->tv_sec = t1->tv_sec - t2->tv_sec;
  r->tv_usec = t1->tv_usec - t2->tv_usec;
  return r;
}

DateTm *date_tm_add (DateTm *t, int millis) {
  DateTm *r = MALLOC(DateTm);
  long int m = t->tv_usec + millis * 1000;
  time_t s = m / 1000000;

  r->tv_sec = t->tv_sec + s;
  r->tv_usec = m - s * 1000000;
  return r;
}

int date_tm_df (DateTm *t1, DateTm *t2) {
  DateTm *r = date_tm_tdf(t1, t2);
  return r->tv_sec * 1000 + r->tv_usec / 1000;
}
