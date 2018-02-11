// Copyright 22-Dec-2017 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dm.h"

static bool contains(char *msg, char *tx) {
  return str_index(msg, tx) != -1;
}

void json_test() {
  printf("Json test\n");

  assert(json_rnull("null"));
  assert(json_rnull("  null   "));
  assert(!json_rnull(""));
  assert(!json_rnull("nuln"));
  assert(str_eq(json_wnull(), "null"));
  assert(json_rbool(" true "));
  assert(json_rbool(" false") == false);

  TRY {
    json_rbool("true and");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading a boolean value in\n'true and'"));
  }_TRY
  TRY {
    json_rbool("xx");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad value reading a boolean value in\n'xx'"));
  }_TRY

  char *s;
  s = json_wbool(true);
  assert(json_rbool(s));
  s = json_wbool(false);
  assert(!json_rbool(s));

  assert(json_rint(" 0 ") == 0);
  assert(json_ruint(" 12 ") == 12);
  assert(json_rdouble(" 0.23 ") == 0.23);
  assert(json_rint(" -0 ") == 0);
  assert(json_rdouble(" -0.0 ") == 0);
  assert(json_rdouble(" -12.11 ") == -12.11);
  assert(json_rdouble(" -12.11e22 ") == -12.11e22);

  TRY {
    json_rint(" 12abc ");
    assert(false);
  } CATCH (e) {
    assert(contains(e, "Bad number"));
  }_TRY
  TRY {
    json_rint(" 12] ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading a number value in\n']"));
  }_TRY
  TRY {
    json_ruint(" 12 }");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading a number value in\n'12 }'"));
  }_TRY
  TRY {
    json_rdouble(" .12");
    assert(false);
  } CATCH (e) {
    assert(contains(e, "Bad start number in\n'.12'"));
  }_TRY
  TRY {
    json_rdouble(" z.12");
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

  assert(str_eq("", json_rstring("  \"\" ")));
  assert(str_eq("a0", json_rstring("  \"a\\u0030\" ")));
  assert(str_eq("a\t\n\"", json_rstring("  \"a\\t\\n\\\"\" ")));

  TRY {
    json_rstring(" \"");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "String does not end with \" in\n' \""));
  }_TRY
  TRY {
    json_rstring(" \"a\" l");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading a string value in\n' \"a\" l"));
  }_TRY
  TRY {
    json_rstring(" \" \\ \" ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad escape sequence in string in\n' \" \\ \" "));
  }_TRY
  TRY {
    json_rstring(" \" \\@ \" ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad escape sequence in string in\n' \" \\@ \" "));
  }_TRY
  TRY {
    json_rstring(" \" \\u30 \" ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Bad hexadecimal unicode in string in\n' \" \\u30 \" "));
  }_TRY

  Arr *a, *a2;
  a = json_rarray("[]");
  assert(arr_size(a) == 0);
  a = json_rarray("[123]");
  assert(arr_size(a) == 1);
  double rsd = json_rdouble(arr_get(a, 0));
  assert(rsd == 123);
  a = json_rarray("[-123.56, true]");
  assert(arr_size(a) == 2);
  rsd = json_rdouble(arr_get(a, 0));
  assert(rsd == -123.56);
  int rs = json_rbool(arr_get(a, 1));
  assert(rs == 1);
  a = json_rarray("[-123.56, true, \"a\"]");
  assert(arr_size(a) == 3);
  assert(str_eq("a", json_rstring(arr_get(a, 2))));

  a = json_rarray("[-123.56, true, [], 56]");
  assert(arr_size(a) == 4);
  rsd = json_rdouble(arr_get(a, 3));
  assert(rsd == 56);
  a = json_rarray(arr_get(a, 2));
  assert(arr_size(a) == 0);

  a = json_rarray(" [-123.56, true, [\"azf\", false], 56]  ");
  assert(arr_size(a) == 4);
  rsd = json_rdouble(arr_get(a, 3));
  assert(rsd == 56);
  a = json_rarray(arr_get(a, 2));
  assert(arr_size(a) == 2);
  rs = json_rbool(arr_get(a, 1));
  assert(rs == 0);
  assert(str_eq("azf", json_rstring(arr_get(a, 0))));

  TRY {
    json_rarray("[-123.56, true, [], 56] h");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Spare characters reading an array value in\n'[-123.56, true, [], 56] h"
    ));
  }_TRY
  TRY {
    json_rarray("[s123.56, true, [], 56] ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Comma missing reading an array value in\n'"));
  }_TRY
  TRY {
    json_rarray(" ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Array does not start with [ in\n'"));
  }_TRY
  TRY {
    json_rarray("[-123.56, true, [], true   ");
    assert(false);
  } CATCH (e) {
    assert(contains(e,
      "Array does not end with ] in\n'"));
  }_TRY

  int arr_eq_str(Arr *a1, Arr *a2) {
    return it_eq_str(arr_to_it(a1), arr_to_it(a2));
  }

  a = arr_new();
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  arr_add(a, json_wbool(1));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  arr_add(a, json_wint(-16));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  arr_add(a, json_wuint(16));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  arr_add(a, json_wdouble(1, 2));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  arr_add(a, json_wstring("caf"));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));

  a2 = arr_new();
  arr_add(a, json_warray(a2));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));
  arr_add(a2, json_wstring("a\n\tzzð"));
  arr_add(a, json_warray(a2));
  assert(arr_eq_str(a, json_rarray(json_warray(a))));

  Map *m;
  m = json_robject("{}");
  assert(arr_size(m) == 0);
  m = json_robject(" {\"a\":123 } ");
  assert(arr_size(m) == 1);
  rsd = json_rdouble(map_get(m, "a"));
  assert(rsd == 123);
  m = json_robject(" {\"a\":123, \"b\":true } ");
  assert(arr_size(m) == 2);
  rs = json_rbool(map_get(m, "b"));
  assert(rs);
  m = json_robject(" {\"a\":123, \"a\":true } ");
  assert(arr_size(m) == 1);
  rs =json_rbool(map_get(m, "a"));
  assert(rs);

  FNM (map_keys_key, Kv, kv) {
    return kv->key;
  }_FN
  It *map_keys(Map *m) {
    return it_map(map_to_it(m), map_keys_key);
  }
  FNM (map_values_value, Kv, kv) {
    return kv->value;
  }_FN
  It *map_values(Map *m) {
    return it_map(map_to_it(m), map_values_value);
  }

  m = map_new();
  assert(
    it_eq_str(map_keys(m),
    map_keys(json_robject(json_wobject(m))))
  );
  assert(
    it_eq_str(map_values(m),
    map_values(json_robject(json_wobject(m))))
  );
  map_put(m, "A", json_wbool(0));
  assert(
    it_eq_str(map_values(m),
    map_values(json_robject(json_wobject(m))))
  );
  map_put(m, "B", json_wint(-34516));
  assert(
    it_eq_str(map_values(m),
    map_values(json_robject(json_wobject(m))))
  );
  map_put(m, "C", json_wdouble(321.189, 2));
  assert(
    it_eq_str(map_values(m),
    map_values(json_robject(json_wobject(m))))
  );
  map_put(m, "D", json_wstring("caf"));
  assert(
    it_eq_str(map_values(m),
    map_values(json_robject(json_wobject(m))))
  );
  map_put(m, "E", json_warray(a));
  assert(
    it_eq_str(map_values(m),
    map_values(json_robject(json_wobject(m))))
  );
  map_put(m, "F", json_wobject(m));
  assert(
    it_eq_str(map_values(m),
    map_values(json_robject(json_wobject(m))))
  );

  printf("    Finshed\n");
}

