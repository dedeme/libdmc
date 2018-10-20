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
  char *r = file_read_new("data/tmp.txt");
  cryp_decryp(&r, key);
  // Map[Js]
  Map *rp = js_ro_new((Js *)r);
  free(r);
  return rp;
}

void cgi_tests(void) {
  puts("Cgi tests");

  cgi_init("data", 10);

  char *key = str_new("cgi_tests");
  cryp_key(&key, cgi_klen());

  cgi_set_key(key);
  cgi_authentication("admin", "deme", 10);

  // Map[Js]
  Map *rp = rp_new(key);
  char *level = js_rs_new(map_get_null(rp, "level"));
  assert(str_eq(level, ""));
  map_free(rp);
  free(level);

  char *pass = str_new("deme");
  cryp_key(&pass, cgi_klen());

  cgi_set_key(key);
  cgi_authentication("admin", pass, 10);
  rp = rp_new(key);

  level = js_rs_new(map_get_null(rp, "level"));
  char *session_id = js_rs_new(map_get_null(rp, "sessionId"));
  assert(str_eq(level, "0"));
  map_free(rp);
  free(level);

  char *key2;
  char *con_id;
  cgi_get_session_data(&key2, &con_id, session_id);
  assert(*key2);
  assert(!*con_id);
  free(key2);
  free(con_id);

  cgi_connect(session_id);
  rp = rp_new(key);
  char *key2b = js_rs_new(map_get_null(rp, "key"));
  char *con_idb = js_rs_new(map_get_null(rp, "connectionId"));
  map_free(rp);

  cgi_get_session_data(&key2, &con_id, session_id);
  assert(*key2);
  assert(*con_id);

  assert(str_eq(key2b, key2));
  assert(str_eq(con_idb, con_id));

  free(key2b);
  free(con_idb);
  free(key2);
  free(con_id);

  cgi_del_session(session_id);
  rp = rp_new(key);
  assert(!map_size(rp));
  map_free(rp);

  cgi_connect(session_id);
  free(session_id);
  rp = rp_new(key);
  key2b = js_rs_new(map_get_null(rp, "key"));
  con_idb = js_rs_new(map_get_null(rp, "connectionId"));
  map_free(rp);

  assert(str_eq(key2b, ""));
  assert(str_eq(con_idb, ""));

  free(key2b);
  free(con_idb);

  cgi_add_user("admin", pass, "u1", "passu1", "1");
  rp = rp_new(key);
  assert(js_rb(map_get_null(rp, "ok")));
  map_free(rp);

  cgi_authentication("u1", "passu1", 10);
  rp = rp_new(key);
  level = js_rs_new(map_get_null(rp, "level"));
  session_id = js_rs_new(map_get_null(rp, "sessionId"));
  map_free(rp);
  assert(str_eq(level, "1"));
  assert(*session_id);
  free(level);
  free(session_id);

  cgi_change_pass("u1", "passu1", "newpassu1");
  rp = rp_new(key);
  assert(js_rb(map_get_null(rp, "ok")));
  map_free(rp);

  cgi_change_level("admin", pass, "u1", "2");
  rp = rp_new(key);
  assert(js_rb(map_get_null(rp, "ok")));
  map_free(rp);

  cgi_authentication("u1", "newpassu1", 10);
  rp = rp_new(key);
  level = js_rs_new(map_get_null(rp, "level"));
  session_id = js_rs_new(map_get_null(rp, "sessionId"));
  map_free(rp);
  assert(str_eq(level, "2"));
  assert(*session_id);
  free(level);
  free(session_id);

  cgi_del_user("admin", pass, "u1");
  rp = rp_new(key);
  assert(js_rb(map_get_null(rp, "ok")));
  map_free(rp);

  cgi_authentication("u1", "newpassu1", 10);
  rp = rp_new(key);
  level = js_rs_new(map_get_null(rp, "level"));
  session_id = js_rs_new(map_get_null(rp, "sessionId"));
  map_free(rp);
  assert(!*level);
  assert(!*session_id);
  free(level);
  free(session_id);

  free(key);
  free(pass);

  cgi_end();

  puts("    Finished");
}


