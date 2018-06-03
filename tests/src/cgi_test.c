// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <assert.h>
#include <locale.h>
#include "dmc/Cgi.h"
#include "dmc/cryp.h"
#include "dmc/ct/Mjson.h"
#include "dmc/Json.h"
#include "dmc/sys.h"
#include "dmc/str.h"
#include "dmc/file.h"
#include "dmc/path.h"

void cgi_test() {
  printf("cgi test\n");

  char *key = cryp_genk(cgi_klen());
  char *dm_key = cryp_key("deme", cgi_klen());
  Mjson *decryp(char *msg) {
    return json_robject((Json *)cryp_decryp(key, msg));
  }

  Cgi *cgi = cgi_new(sys_home(), 1000);
  cgi_set_key(cgi, key);

  Mjson *r = decryp(
    (char *)cgi_authentication(cgi, "admin", dm_key, false)
  );
  assert(str_eq(jmap_gstring(r, "level"), "0"));

  r = decryp((char *)cgi_add_user(cgi, "admin", dm_key, "new", "abc", "1"));
  assert(jmap_gbool(r, "ok"));
  assert(str_eq(jmap_gstring(r, "error"), ""));

  cgi = cgi_new(sys_home(), 1000);
  cgi_set_key(cgi, key);

  r = decryp((char *)cgi_authentication(cgi, "new", "abc", false));
  assert(str_eq(jmap_gstring(r, "level"), "1"));
  r = decryp((char *)cgi_authentication(cgi, "new", "abcd", false));
  assert(str_eq(jmap_gstring(r, "level"), ""));
  cgi_change_level(cgi, "admin", dm_key, "new", "2");
  cgi_change_pass(cgi, "new", "abc", "a");
  r = decryp((char *)cgi_authentication(cgi, "new", "a", false));
  assert(str_eq(jmap_gstring(r, "level"), "2"));

  char *skey = jmap_gstring(r, "key");
  char *session_id = jmap_gstring(r, "sessionId");

  r = decryp((char *)cgi_connect(cgi, session_id));
  char *skey2 = jmap_gstring(r, "key");
  assert(str_eq(skey, skey2));

  char *conId;
  cgi_get_session_data(&skey, &conId, cgi, session_id);
  assert(str_eq(skey, skey2));

  r = decryp((char *)cgi_expired(cgi));
  assert(jmap_gbool(r, "expired"));

  cgi_del_session(cgi, session_id);
  r = decryp((char *)cgi_connect(cgi, session_id));
  assert(str_eq(jmap_gstring(r, "key"), ""));

  file_del(path_cat(sys_home(), "sessions.db", NULL));
  file_del(path_cat(sys_home(), "users.db", NULL));

  printf( "    Finished\n");
}

