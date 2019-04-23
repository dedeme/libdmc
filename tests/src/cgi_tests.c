// Copyright 19-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cgi_tests.h"
#include <assert.h>
#include "dmc/cgi.h"
#include "dmc/cryp.h"

/**************************************************************
 * For passing this test is need to make changes in cgi_ok to *
 * redirect output.                                           *
 **************************************************************/

// Map [Js]
Map *rp_new(char *key) {
  char *r = file_read("data/tmp.txt");
  r = cryp_decryp(r, key);
  // Map[Js]
  return js_ro((Js *)r);
}

void cgi_tests(void) {
  puts("Cgi tests");

  cgi_init("data", 10);

  char *key = cryp_key("cgi_tests", cgi_klen());

  cgi_set_key(key);
  cgi_authentication("admin", "deme", 10);

  // Map[Js]
  Map *rp = rp_new(key);
  char *level = js_rs(opt_get(map_get(rp, "level")));
  assert(str_eq(level, ""));

  char *pass = cryp_key("deme", cgi_klen());

  cgi_set_key(key);
  cgi_authentication("admin", pass, 10);
  rp = rp_new(key);

  level = js_rs(opt_get(map_get(rp, "level")));
  char *session_id = js_rs(opt_get(map_get(rp, "sessionId")));
  assert(str_eq(level, "0"));

  CgiSession *cgiss = opt_get(cgi_get_session(session_id));
  char *key2 = cgiSession_key(cgiss);
  char *con_id = cgiSession_id(cgiss);
  assert(*key2);
  assert(!*con_id);

  cgi_connect(session_id);
  rp = rp_new(key);
  char *key2b = js_rs(opt_get(map_get(rp, "key")));
  char *con_idb = js_rs(opt_get(map_get(rp, "connectionId")));

  cgiss = opt_get(cgi_get_session(session_id));
  key2 = cgiSession_key(cgiss);
  con_id = cgiSession_id(cgiss);
  assert(*key2);
  assert(*con_id);

  assert(str_eq(key2b, key2));
  assert(str_eq(con_idb, con_id));

  cgi_del_session(session_id);
  rp = rp_new(key);
  assert(!map_size(rp));

  cgi_connect(session_id);
  rp = rp_new(key);
  key2b = js_rs(opt_get(map_get(rp, "key")));
  con_idb = js_rs(opt_get(map_get(rp, "connectionId")));

  assert(str_eq(key2b, ""));
  assert(str_eq(con_idb, ""));

  cgi_add_user("admin", pass, "u1", "passu1", "1");
  rp = rp_new(key);
  assert(js_rb(opt_get(map_get(rp, "ok"))));

  cgi_authentication("u1", "passu1", 10);
  rp = rp_new(key);
  level = js_rs(opt_get(map_get(rp, "level")));
  session_id = js_rs(opt_get(map_get(rp, "sessionId")));
  assert(str_eq(level, "1"));
  assert(*session_id);

  cgi_change_pass("u1", "passu1", "newpassu1");
  rp = rp_new(key);
  assert(js_rb(opt_get(map_get(rp, "ok"))));

  cgi_change_level("admin", pass, "u1", "2");
  rp = rp_new(key);
  assert(js_rb(opt_get(map_get(rp, "ok"))));

  cgi_authentication("u1", "newpassu1", 10);
  rp = rp_new(key);
  level = js_rs(opt_get(map_get(rp, "level")));
  session_id = js_rs(opt_get(map_get(rp, "sessionId")));
  assert(str_eq(level, "2"));
  assert(*session_id);

  cgi_del_user("admin", pass, "u1");
  rp = rp_new(key);
  assert(js_rb(opt_get(map_get(rp, "ok"))));

  cgi_authentication("u1", "newpassu1", 10);
  rp = rp_new(key);
  level = js_rs(opt_get(map_get(rp, "level")));
  session_id = js_rs(opt_get(map_get(rp, "sessionId")));
  assert(!*level);
  assert(!*session_id);

  puts("    Finished");
}


