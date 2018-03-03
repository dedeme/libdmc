// Copyright 12-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/all.h"
#include <locale.h>


void cgi_test() {
  printf("cgi test\n");

  char *key = cryp_genk(cgi_klen());
  char *dm_key = cryp_key("deme", cgi_klen());
  Map/*Json*/ *decryp(char *msg) { return json_robject(cryp_decryp(key, msg)); }

  Cgi *cgi = cgi_new(sys_home(), 1000);
  cgi_set_key(cgi, key);

  Map/*Json*/ *r = decryp(cgi_authentication(cgi, "admin", dm_key, false));
  assert(!strcmp(jmap_gstring(r, "level"), "0"));

  r = decryp(cgi_add_user(cgi, "admin", dm_key, "new", "abc", "1"));
  assert(jmap_gbool(r, "ok"));
  assert(!strcmp(jmap_gstring(r, "error"), ""));

  cgi = cgi_new(sys_home(), 1000);
  cgi_set_key(cgi, key);

  r = decryp(cgi_authentication(cgi, "new", "abc", false));
  assert(!strcmp(jmap_gstring(r, "level"), "1"));
  r = decryp(cgi_authentication(cgi, "new", "abcd", false));
  assert(!strcmp(jmap_gstring(r, "level"), ""));
  cgi_change_level(cgi, "admin", dm_key, "new", "2");
  cgi_change_pass(cgi, "new", "abc", "a");
  r = decryp(cgi_authentication(cgi, "new", "a", false));
  assert(!strcmp(jmap_gstring(r, "level"), "2"));

  char *skey = jmap_gstring(r, "key");
  char *session_id = jmap_gstring(r, "sessionId");

  r = decryp(cgi_connect(cgi, session_id));
  char *skey2 = jmap_gstring(r, "key");
  assert(!strcmp(skey, skey2));

  char *conId;
  cgi_get_session_data(&skey, &conId, cgi, session_id);
  assert(!strcmp(skey, skey2));

  r = decryp(cgi_expired(cgi));
  assert(jmap_gbool(r, "expired"));

  cgi_del_session(cgi, session_id);
  r = decryp(cgi_connect(cgi, session_id));
  assert(!strcmp(jmap_gstring(r, "key"), ""));

  file_del(path_cat(sys_home(), "sessions.db", NULL));
  file_del(path_cat(sys_home(), "users.db", NULL));

  printf( "    Finished\n");
}

