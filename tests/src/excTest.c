/*
 * Copyright 02-May-2017 ºDeme
 * GNU General Public License - V3 <http://www.gnu.org/licenses/>
 */
#include "dm.h"

static bool contains(char *msg, char *tx) {
  return str_index(msg, tx) != -1;
}

static char *f1(int v) {
  if (v) {
    THROW "From f1" _THROW;
    assert(0);
  }
  return "ok";
}

static char *f0(int v) {
  char *r;
  TRY {
    r = f1(v);
    if (v) {
      assert(0);
    }
  } CATCH(e) {
    assert(contains(e, "From f1"));
    THROW e _THROW;
    assert(0);
  }_TRY
  return r;
}

void exc_test() {
  puts("exc test");

  TRY {
    char *r = f0(1);
    assert(0);
    puts(r);
  } CATCH(e) {
    assert(contains(e, "From f1"));
  } _TRY

  TRY {
    char *r = f1(1);
    assert(0);
    puts(r);
  } CATCH(e) {
    assert(contains(e, "From f1"));
  } _TRY

  TRY {
    char *r = f0(0);
    assert(!strcmp(r, "ok"));
  } CATCH(e) {
    assert(0);
  } _TRY

  TRY {
    char *r = f1(0);
    assert(!strcmp(r, "ok"));
  } CATCH(e) {
    assert(0);
  } _TRY

 puts("    Finised");
}
