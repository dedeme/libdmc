// Copyright 19-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "cgi_tests.h"
#include <assert.h>
#include "dmc/cgi.h"
#include "dmc/cryp.h"



// Map [Js]
Map *rp_new(char *key, char *rp) {
  rp = cryp_decryp(rp, key);
  // Map[Js]
  return js_ro((Js *)rp);
}

void cgi_tests(void) {
  puts("Cgi tests");

  cgi_init("data", 10);

  char *key = cryp_key("cgi_tests", cgi_klen());

  cgi_set_key(key);
  char *r = cgi_authentication("admin", "deme", 10);

  // Map[Js]
  Map *rp = rp_new(key, r);
  char *level = js_rs(opt_get(map_get(rp, "level")));
  assert(str_eq(level, ""));

  char *pass = cryp_key("deme", cgi_klen());

  cgi_set_key(key);
  r = cgi_authentication("admin", pass, 10);
  rp = rp_new(key, r);

  level = js_rs(opt_get(map_get(rp, "level")));
  char *session_id = js_rs(opt_get(map_get(rp, "sessionId")));
  assert(str_eq(level, "0"));

  CgiSession *cgiss = opt_get(cgi_get_session(session_id));
  char *key2 = cgiSession_com_key(cgiss);
  char *key3 = cgiSession_con_key(cgiss);
  char *suser = cgiSession_user(cgiss);
  char *slevel = cgiSession_level(cgiss);
  assert(*key2);
  assert(*key3);
  assert(str_eq(suser, "admin"));
  assert(str_eq(slevel, "0"));

  r = cgi_connect(session_id);
  rp = rp_new(key, r);
  char *key2b = js_rs(opt_get(map_get(rp, "key")));

  cgiss = opt_get(cgi_get_session(session_id));
  key2 = cgiSession_com_key(cgiss);
  key3 = cgiSession_con_key(cgiss);
  suser = cgiSession_user(cgiss);
  slevel = cgiSession_level(cgiss);
  assert(*key2);
  assert(*key3);
  assert(str_eq(suser, "admin"));
  assert(str_eq(slevel, "0"));

  assert(str_eq(key2b, key2));

  r = cgi_del_session(session_id);
  rp = rp_new(key, r);
  assert(!map_size(rp));

  r = cgi_connect(session_id);
  rp = rp_new(key, r);
  key2b = js_rs(opt_get(map_get(rp, "key")));

  assert(str_eq(key2b, ""));

  r = cgi_add_user("admin", pass, "u1", "passu1", "1");
  rp = rp_new(key, r);
  assert(js_rb(opt_get(map_get(rp, "ok"))));

  r = cgi_authentication("u1", "passu1", 10);
  rp = rp_new(key, r);
  level = js_rs(opt_get(map_get(rp, "level")));
  session_id = js_rs(opt_get(map_get(rp, "sessionId")));
  assert(str_eq(level, "1"));
  assert(*session_id);

  r = cgi_change_pass("u1", "passu1", "newpassu1");
  rp = rp_new(key, r);
  assert(js_rb(opt_get(map_get(rp, "ok"))));

  r = cgi_change_level("admin", pass, "u1", "2");
  rp = rp_new(key, r);
  assert(js_rb(opt_get(map_get(rp, "ok"))));

  r = cgi_authentication("u1", "newpassu1", 10);
  rp = rp_new(key, r);
  level = js_rs(opt_get(map_get(rp, "level")));
  session_id = js_rs(opt_get(map_get(rp, "sessionId")));
  assert(str_eq(level, "2"));
  assert(*session_id);

  r = cgi_del_user("admin", pass, "u1");
  rp = rp_new(key, r);
  assert(js_rb(opt_get(map_get(rp, "ok"))));

  r = cgi_authentication("u1", "newpassu1", 10);
  rp = rp_new(key, r);
  level = js_rs(opt_get(map_get(rp, "level")));
  session_id = js_rs(opt_get(map_get(rp, "sessionId")));
  assert(!*level);
  assert(!*session_id);

  puts("    Finished");
}


