// Copyright 1-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include <gc.h>
#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"
#include "dmc/ct/Mjson.h"
#include "dmc/ct/Ojson.h"
#include "dmc/ct/Achar.h"
#include "dmc/ct/Ichar.h"
#include "dmc/ct/Mchar.h"
#include "dmc/ct/Ijson.h"
#include "dmc/ct/Ochar.h"
#include "dmc/It.h"
#include "dmc/Tuples.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

static bool contains(char *msg, char *tx) {
  return str_index(msg, tx) != -1;
}

void json_test() {
  printf("Json test\n");

  assert(json_rnull((Json*)"null"));
  assert(json_rnull((Json*)"  null   "));
  assert(!json_rnull((Json*)""));
  assert(!json_rnull((Json*)"nuln"));
  assert(str_eq((char *)json_wnull(), "null"));
  assert(json_rbool((Json*)" true "));
  assert(json_rbool((Json*)" false") == false);

  TRY {
    json_rbool((Json*)"true and");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading a boolean value in\n'true and'"));
  }_TRY
  TRY {
    json_rbool((Json*)"xx");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad value reading a boolean value in\n'xx'"));
  }_TRY

  Json *s;
  s = json_wbool(true);
  assert(json_rbool(s));
  s = json_wbool(false);
  assert(!json_rbool(s));

  assert(json_rint((Json*)" 0 ") == 0);
  assert(json_ruint((Json*)" 12 ") == 12);
  assert(json_rdouble((Json*)" 0.23 ") == 0.23);
  assert(json_rint((Json*)" -0 ") == 0);
  assert(json_rdouble((Json*)" -0.0 ") == 0);
  assert(json_rdouble((Json*)" -12.11 ") == -12.11);
  assert(json_rdouble((Json*)" -12.11e22 ") == -12.11e22);

  TRY {
    json_rint((Json*)" 12abc ");
    assert(false);
  } CATCH (e) {
    assert(contains(e, "Bad number"));
  }_TRY
  TRY {
    json_rint((Json*)" 12] ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading a number value in\n']"));
  }_TRY
  TRY {
    json_ruint((Json*)" 12 }");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading a number value in\n'12 }'"));
  }_TRY
  TRY {
    json_rdouble((Json*)" .12");
    assert(false);
  } CATCH (e) {
    assert(contains(e, "Bad start number in\n'.12'"));
  }_TRY
  TRY {
    json_rdouble((Json*)" z.12");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad start number in\n'z.12'"));
  }_TRY

  s = json_wuint(0);
  assert(json_ruint(s) == 0);
  s = json_wuint(254);
  assert(json_ruint(s) == 254);
  s = json_wint(0);
  assert(json_rint(s) == 0);
  s = json_wint(254);
  assert(json_rint(s) == 254);
  s = json_wint(-1100);
  assert(json_rint(s) == -1100);
  s = json_wdouble(0.0, 5);
  assert(json_rdouble(s) == 0);
  s = json_wdouble(-0.0, 0);
  assert(json_rdouble(s) == 0);
  s = json_wdouble(-.0, 2);
  assert(json_rdouble(s) == 0);
  s = json_wdouble(1.045, 3);
  assert(json_rdouble(s) == 1.045);
  s = json_wdouble(-21.045, 3);
  assert(json_rdouble(s) == -21.045);
  s = json_wdouble(-21.045, 2);
  assert(json_rdouble(s) == -21.05);

  assert(str_eq("", json_rstring((Json*)"  \"\" ")));
  assert(str_eq("a0", json_rstring((Json*)"  \"a\\u0030\" ")));
  assert(str_eq("a\t\n\"", json_rstring((Json*)"  \"a\\t\\n\\\"\" ")));

  TRY {
    json_rstring((Json*)" \"");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "String does not end with \" in\n' \""));
  }_TRY
  TRY {
    json_rstring((Json*)" \"a\" l");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading a string value in\n' \"a\" l"));
  }_TRY
  TRY {
    json_rstring((Json*)" \" \\ \" ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad escape sequence in string in\n' \" \\ \" "));
  }_TRY
  TRY {
    json_rstring((Json*)" \" \\@ \" ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad escape sequence in string in\n' \" \\@ \" "));
  }_TRY
  TRY {
    json_rstring((Json*)" \" \\u30 \" ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad hexadecimal unicode in string in\n' \" \\u30 \" "));
  }_TRY

  Ajson *a, *a2;
  a = json_rarray((Json*)"[]");
  assert(ajson_size(a) == 0);
  a = json_rarray((Json*)"[123]");
  assert(ajson_size(a) == 1);
  double rsd = json_rdouble(ajson_get(a, 0));
  assert(rsd == 123);
  a = json_rarray((Json*)"[-123.56, true]");
  assert(ajson_size(a) == 2);
  rsd = json_rdouble(ajson_get(a, 0));
  assert(rsd == -123.56);
  int rs = json_rbool(ajson_get(a, 1));
  assert(rs == 1);
  a = json_rarray((Json*)"[-123.56, true, \"a\"]");
  assert(ajson_size(a) == 3);
  assert(str_eq("a", json_rstring(ajson_get(a, 2))));

  a = json_rarray((Json*)"[-123.56, true, [], 56]");
  assert(ajson_size(a) == 4);
  rsd = json_rdouble(ajson_get(a, 3));
  assert(rsd == 56);
  a = json_rarray(ajson_get(a, 2));
  assert(ajson_size(a) == 0);

  a = json_rarray((Json*)" [-123.56, true, [\"azf\", false], 56]  ");
  assert(ajson_size(a) == 4);
  rsd = json_rdouble(ajson_get(a, 3));
  assert(rsd == 56);
  a = json_rarray(ajson_get(a, 2));
  assert(ajson_size(a) == 2);
  rs = json_rbool(ajson_get(a, 1));
  assert(rs == 0);
  assert(str_eq("azf", json_rstring(ajson_get(a, 0))));

  TRY {
    json_rarray((Json*)"[-123.56, true, [], 56] h");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading an array value in\n'[-123.56, true, [], 56] h"
    ));
  }_TRY
  TRY {
    json_rarray((Json*)"[s123.56, true, [], 56] ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Comma missing reading an array value in\n'"));
  }_TRY
  TRY {
    json_rarray((Json*)" ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Array does not start with [ in\n'"));
  }_TRY
  TRY {
    json_rarray((Json*)"[-123.56, true, [], true   ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Array does not end with ] in\n'"));
  }_TRY

  bool arr_eq_str(Ajson *a1, Ajson *a2) {
    return ichar_eq(achar_to_it((Achar *)a1), achar_to_it((Achar *)a2));
  }

  a = ajson_new();
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  ajson_add(a, json_wbool(1));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  ajson_add(a, json_wint(-16));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  ajson_add(a, json_wuint(16));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  ajson_add(a, json_wdouble(1, 2));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  ajson_add(a, json_wstring("caf"));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));

  a2 = ajson_new();
  ajson_add(a, json_warray(a2));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  ajson_add(a2, json_wstring("a\n\tzzð"));
  ajson_add(a, json_warray(a2));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));

  Mjson *m;
  m = json_robject((Json*)"{}");
  assert(mjson_size(m) == 0);
  m = json_robject((Json*)" {\"a\":123 } ");
  assert(mjson_size(m) == 1);
  rsd = json_rdouble(ojson_value(mjson_get(m, "a")));
  assert(rsd == 123);
  m = json_robject((Json*)" {\"a\":123, \"b\":true } ");
  assert(mjson_size(m) == 2);
  rs = json_rbool(ojson_value(mjson_get(m, "b")));
  assert(rs);
  m = json_robject((Json*)" {\"a\":123, \"a\":true } ");
  assert(mjson_size(m) == 1);
  rs =json_rbool(ojson_value(mjson_get(m, "a")));
  assert(rs);

  m = mjson_new();
  assert(
    ichar_eq(mjson_keys(m),
    mjson_keys(json_robject(json_wobject(m))))
  );
  assert(
    ichar_eq((Ichar *)mjson_values(m),
    (Ichar *)mjson_values(json_robject(json_wobject(m))))
  );

  mjson_put(m, "A", json_wbool(0));
  assert(
    ichar_eq((Ichar *)mjson_values(m),
    (Ichar *)mjson_values(json_robject(json_wobject(m))))
  );
  mjson_put(m, "B", json_wint(-34516));
  assert(
    ichar_eq((Ichar *)mjson_values(m),
    (Ichar *)mjson_values(json_robject(json_wobject(m))))
  );
  mjson_put(m, "C", json_wdouble(321.189, 2));
  assert(
    ichar_eq((Ichar *)mjson_values(m),
    (Ichar *)mjson_values(json_robject(json_wobject(m))))
  );
  mjson_put(m, "D", json_wstring("caf"));
  assert(
    ichar_eq((Ichar *)mjson_values(m),
    (Ichar *)mjson_values(json_robject(json_wobject(m))))
  );
  mjson_put(m, "E", json_warray(a));
  assert(
    ichar_eq((Ichar *)mjson_values(m),
   (Ichar *)mjson_values(json_robject(json_wobject(m))))
  );
  mjson_put(m, "F", json_wobject(m));
  assert(
    ichar_eq((Ichar *)mjson_values(m),
    (Ichar *)mjson_values(json_robject(json_wobject(m))))
  );

  Achar *astr1 = achar_new();
  achar_add(astr1, "one");
  achar_add(astr1, "two");
  Achar *astr2 = achar_new();
  achar_add(astr2, "three");
  Achar *astr3 = achar_new();

  a = ajson_new();
  jarr_aarray(a, (Arr *)astr1, (Json*(*)(void*))json_wstring);
  jarr_aarray(a, (Arr *)astr2, (Json*(*)(void*))json_wstring);
  jarr_aarray(a, (Arr *)astr3, (Json*(*)(void*))json_wstring);

  Achar *astrb = (Achar *)jarr_garray(a, 0, (void*(*)(Json*))json_rstring);
  assert(str_eq(achar_get(astrb, 0), "one"));
  assert(str_eq(achar_get(astrb, 1), "two"));

  astrb = (Achar *)jarr_garray(a, 1, (void*(*)(Json*))json_rstring);
  assert(str_eq(achar_get(astrb, 0), "three"));

  astrb = (Achar *)jarr_garray(a, 2, (void*(*)(Json*))json_rstring);
  assert(achar_size(astrb) == 0);

  jarr_sarray(a, 2, (Arr *)astr1, (Json*(*)(void*))json_wstring);
  astrb = (Achar *)jarr_garray(a, 2, (void*(*)(Json*))json_rstring);
  assert(str_eq(achar_get(astrb, 0), "one"));
  assert(str_eq(achar_get(astrb, 1), "two"));

  m = mjson_new();
  jmap_parray(m, "a", (Arr *)astr1, (Json*(*)(void*))json_wstring);
  jmap_parray(m, "b", (Arr *)astr2, (Json*(*)(void*))json_wstring);
  jmap_parray(m, "c", (Arr *)astr3, (Json*(*)(void*))json_wstring);

  astrb = (Achar *)jmap_garray(m, "a", (void*(*)(Json*))json_rstring);
  assert(str_eq(achar_get(astrb, 0), "one"));
  assert(str_eq(achar_get(astrb, 1), "two"));

  astrb = (Achar *)jmap_garray(m, "b", (void*(*)(Json*))json_rstring);
  assert(str_eq(achar_get(astrb, 0), "three"));

  astrb = (Achar *)jmap_garray(m, "c", (void*(*)(Json*))json_rstring);
  assert(achar_size(astrb) == 0);

  jmap_parray(m, "c", (Arr *)astr1, (Json*(*)(void*))json_wstring);
  astrb = (Achar *)jmap_garray(m, "c", (void*(*)(Json*))json_rstring);
  assert(str_eq(achar_get(astrb, 0), "one"));
  assert(str_eq(achar_get(astrb, 1), "two"));

  Mchar *mstr1 = mchar_new();
  mchar_put(mstr1, "one", "1");
  mchar_put(mstr1, "two", "2");

  Mchar *mstr2 = mchar_new();
  mchar_put(mstr2, "three", "3");

  Mchar *mstr3 = mchar_new();

  a = ajson_new();
  jarr_aobject(a, (Map *)mstr1, (Json*(*)(void*))json_wstring);
  jarr_aobject(a, (Map *)mstr2, (Json*(*)(void*))json_wstring);
  jarr_aobject(a, (Map *)mstr3, (Json*(*)(void*))json_wstring);

  Mchar *mstrb = (Mchar *)jarr_gobject(a, 0, (void*(*)(Json*))json_rstring);
  assert(str_eq(ochar_value(mchar_get(mstrb, "one")), "1"));
  assert(str_eq(ochar_value(mchar_get(mstrb, "two")), "2"));

  mstrb = (Mchar *)jarr_gobject(a, 1, (void*(*)(Json*))json_rstring);
  assert(str_eq(ochar_value(mchar_get(mstrb, "three")), "3"));

  mstrb = (Mchar *)jarr_gobject(a, 2, (void*(*)(Json*))json_rstring);
  assert(mchar_size(mstrb) == 0);

  m = mjson_new();
  jmap_pobject(m, "a", (Map *)mstr1, (Json*(*)(void*))json_wstring);
  jmap_pobject(m, "b", (Map *)mstr2, (Json*(*)(void*))json_wstring);
  jmap_pobject(m, "c", (Map *)mstr3, (Json*(*)(void*))json_wstring);

  mstrb = (Mchar *)jmap_gobject(m, "a", (void*(*)(Json*))json_rstring);
  assert(str_eq(ochar_value(mchar_get(mstrb, "one")), "1"));
  assert(str_eq(ochar_value(mchar_get(mstrb, "two")), "2"));

  mstrb = (Mchar *)jmap_gobject(m, "b", (void*(*)(Json*))json_rstring);
  assert(str_eq(ochar_value(mchar_get(mstrb, "three")), "3"));

  mstrb = (Mchar *)jmap_gobject(m, "c", (void*(*)(Json*))json_rstring);
  assert(mchar_size(mstrb) == 0);

  jmap_pobject(m, "c", (Map *)mstr1, (Json*(*)(void*))json_wstring);
  mstrb = (Mchar *)jmap_gobject(m, "c", (void*(*)(Json*))json_rstring);
  assert(str_eq(ochar_value(mchar_get(mstrb, "one")), "1"));
  assert(str_eq(ochar_value(mchar_get(mstrb, "two")), "2"));

  printf("    Finshed\n");
}

