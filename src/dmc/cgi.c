// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/cgi.h"
#include "dmc/ct/Achar.h"
#include "dmc/ct/Ochar.h"
#include "dmc/ct/Mjson.h"
#include "dmc/ct/Ajson.h"
#include "dmc/rnd.h"
#include "dmc/cryp.h"
#include "dmc/str.h"
#include "dmc/file.h"
#include "dmc/path.h"
#include "dmc/Json.h"
#include "dmc/Arr.h"
#include "dmc/Date.h"
#include "dmc/It.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

static size_t klen = 300;
static time_t t_no_expiration = 2592000; // seconds == 30 days
static char *deme_key =
    "nkXliX8lg2kTuQSS/OoLXCk8eS4Fwmc+N7l6TTNgzM1vdKewO0cjok51vcdl"
    "OKVXyPu83xYhX6mDeDyzapxL3dIZuzwyemVw+uCNCZ01WDw82oninzp88Hef"
    "bn3pPnSMqEaP2bOdX+8yEe6sGkc3IO3e38+CqSOyDBxHCqfrZT2Sqn6SHWhR"
    "KqpJp4K96QqtVjmXwhVcST9l+u1XUPL6K9HQfEEGMGcToMGUrzNQxCzlg2g+"
    "Hg55i7iiKbA0ogENhEIFjMG+wmFDNzgjvDnNYOaPTQ7l4C8aaPsEfl3sugiw";

struct cgi_Cgi {
  char *fkey; // key to encrypt files
  char *key;  // key to communications. Must be set before to call ok or error
  char *home;
  time_t t_expiration;
};

static Cgi *cgi = NULL;

static void write_users(Achar *users);
static void put_user(char *user, char *key, char *level);
static void write_sessions(Arr/*Ajson*/ *sessions);

void cgi_init(char *home, time_t t_expiration) {
  rnd_init();

  cgi = MALLOC(Cgi);

  cgi->fkey = cryp_key(deme_key, str_len(deme_key));
  cgi->key = NULL;
  cgi->home = home;
  cgi->t_expiration = t_expiration;

  if (!file_exists(home)) {
    file_mkdir(home);
  }

  char *fusers = path_cat(home, "users.db", NULL);
  if (!file_exists(fusers)) {
    write_users(achar_new());
    put_user("admin", deme_key, "0");
    write_sessions(arr_new());
  }
}

// User ----------------------------------------------------

static void write_users(Achar *users) {
  char *path = path_cat(cgi->home, "users.db", NULL);
  Ajson *tmp = ajson_new();
  EACH(users, char, udata) {
    ajson_add(tmp, json_wstring(udata));
  }_EACH
  file_write(path, cryp_cryp(cgi->fkey, (char *)json_warray(tmp)));
}

static Achar *read_users() {
  char *path = path_cat(cgi->home, "users.db", NULL);
  Ajson *tmp = json_rarray((Json *)cryp_decryp(cgi->fkey, file_read(path)));
  Achar *r = achar_new();
  EACH(tmp, Json, js) {
    achar_add(r, json_rstring(js));
  }_EACH
  return r;
}

static Achar *remove_user(Achar *users, char *user) {
  Achar *r = achar_new();
  EACH(users, char, udata) {
    Achar *parts = str_csplit(udata, ':');
    if (!str_eq(user, achar_get(parts, 0))) {
      achar_add(r, udata);
    }
  }_EACH
  return r;
}

static void del_user(char *user) {
  write_users(remove_user(read_users(), user));
}

static void put_user(char *user, char *key, char *level) {
  Achar *users = remove_user(read_users(cgi), user);
  char *kkey = cryp_key(key, klen);
  achar_add(users, str_printf("%s:%s:%s", user, kkey, level));
  write_users(users);
}

// If fails r is opt_null()
static Ochar *check_user(char *id, char *key) {
  char *kkey = cryp_key(key, klen);
  Achar *users = read_users();

  EACH(users, char, udata) {
    Achar *parts = str_csplit(udata, ':');
    if (
      str_eq(achar_get(parts, 0), id) &&
      str_eq(achar_get(parts, 1), kkey)
    ) {
      return ochar_new(achar_get(parts, 2));
    }
  }_EACH
  return ochar_null();
}

static bool change_level(char *user, char *level) {
  Achar *users = read_users();

  bool r = false;
  RANGE0(i, achar_size(users)){
    char *udata = achar_get(users, i);
    Achar *parts = str_csplit(udata, ':');
    if (str_eq(achar_get(parts, 0), user)) {
      char *new_udata = str_printf("%s:%s:%s",
        achar_get(parts, 0), achar_get(parts, 1), level
      );
      achar_set(users, i, new_udata);
      r = true;
      break;
    }
  }_RANGE
  if (r) {
    write_users(users);
  }
  return r;
}

static bool change_pass(char *user, char *old_pass, char *new_pass) {
  char *kold = cryp_key(old_pass, klen);
  char *knew = cryp_key(new_pass, klen);
  Achar *users = read_users();

  bool r = false;
  RANGE0(i, achar_size(users)){
    char *udata = achar_get(users, i);
    Achar *parts = str_csplit(udata, ':');
    if (
      str_eq(achar_get(parts, 0), user) &&
      str_eq(achar_get(parts, 1), kold)
    ) {
      char *new_udata = str_printf("%s:%s:%s",
        achar_get(parts, 0), knew, achar_get(parts, 2)
      );
      achar_set(users, i, new_udata);
      r = true;
      break;
    }
  }_RANGE
  if (r) {
    write_users(users);
  }
  return r;
}

// Session -------------------------------------------------

static void write_sessions(Arr/*Ajson*/ *sessions) {
  char *path = path_cat(cgi->home, "sessions.db", NULL);
  Ajson *tmp = ajson_new();
  EACH(sessions, Ajson, sdata) {
    ajson_add(tmp, json_warray(sdata));
  }_EACH
  file_write(path, cryp_cryp(cgi->fkey, (char *)json_warray(tmp)));
}

static Arr/*Arr[Json]*/ *read_sessions() {
  char *path = path_cat(cgi->home, "sessions.db", NULL);
  Ajson *tmp = json_rarray((Json*)cryp_decryp(cgi->fkey, file_read(path)));
  Arr/*char*/ *r = arr_new();
  EACH(tmp, Json, js) {
    arr_add(r, json_rarray(js));
  }_EACH
  return r;
}

// If expiration is 0 tNoExpiration is used
static void add_session(
  char *session_id, char *user, char *key, time_t expiration
) {
  time_t lapse = expiration ? expiration : t_no_expiration;
  Date time = date_now() + lapse;

  Ajson *row = ajson_new();
  jarr_astring(row, session_id);  //0
  jarr_astring(row, user);       //1
  jarr_astring(row, key);        //2
  jarr_astring(row, "");         //3
  jarr_auint(row, time);         //4
  jarr_auint(row, lapse);        //5

  /**/FNP(filter, Ajson, row) {
  /**/  return !str_eq(user, json_rstring(ajson_get(row, 1)));
  /**/}_FN
  write_sessions(
    arr_from_it(it_cat(
      it_unary(row),
      it_filter(arr_to_it(read_sessions()), filter)
    ))
  );
}

// In session.db:
// Fields are: sessionId:key:time:lapse
// If identification fails returns ""
static void read_session(
  char **key, char** conId, char *session_id
) {
  *key = "";
  *conId = "";
  Date now = date_now();

  /**/FNP(filter, Arr/*Json*/, row) {
  /**/  return json_ruint(arr_get(row, 4)) >= now;
  /**/}_FN
  /**/FNM(map, Arr/*Json*/, row) {
  /**/  if (str_eq(json_rstring(arr_get(row, 0)), session_id)){
  /**/    *key = json_rstring(arr_get(row, 2));
  /**/    *conId = json_rstring(arr_get(row, 3));
  /**/  }
  /**/  arr_set(row, 4, json_wuint(now + json_ruint(arr_get(row, 5))));
  /**/  return row;
  /**/}_FN
  write_sessions(
    arr_from_it(it_map(it_filter(arr_to_it(read_sessions()), filter), map))
  );
}

static void del_session(char *session_id) {
  /**/FNP(filter, Arr/*Json*/, row) {
  /**/  return !str_eq(session_id, json_rstring(arr_get(row, 0)));
  /**/}_FN
  write_sessions(
    arr_from_it(it_filter(arr_to_it(read_sessions()), filter))
  );
}

static void set_connection_id(char *session_id, char *con_id) {
  /**/FNM(map, Arr/*Json*/, row) {
  /**/  if (str_eq(json_rstring(arr_get(row, 0)), session_id)){
  /**/    arr_set(row, 3, json_wstring(con_id));
  /**/  }
  /**/  return row;
  /**/}_FN
  write_sessions(
    arr_from_it(it_map(arr_to_it(read_sessions()), map))
  );
}

// Public interface ----------------------------------------

int cgi_klen(void) {
  return klen;
}

char *cgi_home(void) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  return cgi->home;
}

void cgi_set_key(char *k) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  cgi->key = k;
}

void cgi_get_session_data(
  char **key, char **connectionId, char *session_id
) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  read_session(key, connectionId, session_id);
}

CgiRp *cgi_add_user(
  char *admin, char *akey, char *user, char *ukey, char *level
){
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  Mjson *m = mjson_new();
  Ochar *alevel = check_user(admin, akey);
  if (!ochar_is_null(alevel) && str_eq(ochar_value(alevel), "0")) {
    put_user(user, ukey, level);
    jmap_pbool(m, "ok", true);
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(m);
}

CgiRp *cgi_del_user(char *admin, char *akey, char *user) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  Mjson *m = mjson_new();
  Ochar *alevel = check_user(admin, akey);
  if (!ochar_is_null(alevel) && str_eq(ochar_value(alevel), "0")) {
    del_user(user);
    jmap_pbool(m, "ok", true);
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(m);
}

CgiRp *cgi_change_level(
  char *admin, char *akey, char *user, char *level
) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  Mjson *m = mjson_new();
  Ochar *alevel = check_user(admin, akey);
  if (!ochar_is_null(alevel) && str_eq(ochar_value(alevel), "0")) {
    if (change_level(user, level)) {
      jmap_pbool(m, "ok", true);
    } else {
      jmap_pbool(m, "ok", false);
    }
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(m);
}

CgiRp *cgi_change_pass(char *user, char *key, char *new_key) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  Mjson *m = mjson_new();
  if (change_pass(user, key, new_key)) {
    jmap_pbool(m, "ok", true);
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(m);
}


CgiRp *cgi_del_session(char *session_id) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  del_session(session_id);
  return cgi_ok(mjson_new());
}

CgiRp *cgi_authentication(char *user, char *key, bool expiration) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  Mjson *rp = mjson_new();

  Ochar *level = check_user(user, key);
  if (!ochar_is_null(level)) {
    char *session_id = cryp_genk(klen);
    char *key = cryp_genk(klen);
    add_session(
      session_id, user, key, expiration ? cgi->t_expiration : 0
    );
    jmap_pstring(rp, "level", ochar_value(level));
    jmap_pstring(rp, "sessionId", session_id);
    jmap_pstring(rp, "key", key);
  } else {
    jmap_pstring(rp, "level", "");
    jmap_pstring(rp, "sessionId", "");
    jmap_pstring(rp, "key", "");
  }

  return cgi_ok(rp);
}

CgiRp *cgi_connect(char *session_id) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  set_connection_id(session_id, cryp_genk(klen));

  char *key;
  char *connectionId;
  read_session(&key, &connectionId, session_id);

  Mjson *rp = mjson_new();
  jmap_pstring(rp, "key", key);
  jmap_pstring(rp, "connectionId", connectionId);
  return cgi_ok(rp);
}

CgiRp *cgi_ok(Mjson *data) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  if (!cgi->key) {
    return (CgiRp *)"cgi_ok: this->key not set";
  }
  return (CgiRp *)cryp_cryp(cgi->key, (char *)json_wobject(data));
}

CgiRp *cgi_error(char *msg) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  if (!cgi->key) {
    return (CgiRp *)"cgi_ok: this->key not set";
  }

  Mjson *data = mjson_new();
  jmap_pstring(data, "error", msg);
  return (CgiRp *)cryp_cryp(cgi->key, (char *)json_wobject(data));
}

CgiRp *cgi_expired(void) {
  if (!cgi) {
    exc_illegal_state("'cgi' has not been intialized");
  }
  Mjson *data = mjson_new();
  jmap_pbool(data, "expired", true);
  return cgi_ok(data);
}
