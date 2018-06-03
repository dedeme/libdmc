// Copyright 02-Jun-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/Cgi.h"
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
#include "dmc/DEFS.h"

static size_t klen = 300;
static time_t t_no_expiration = 2592000; // seconds == 30 days
static char *deme_key =
    "nkXliX8lg2kTuQSS/OoLXCk8eS4Fwmc+N7l6TTNgzM1vdKewO0cjok51vcdl"
    "OKVXyPu83xYhX6mDeDyzapxL3dIZuzwyemVw+uCNCZ01WDw82oninzp88Hef"
    "bn3pPnSMqEaP2bOdX+8yEe6sGkc3IO3e38+CqSOyDBxHCqfrZT2Sqn6SHWhR"
    "KqpJp4K96QqtVjmXwhVcST9l+u1XUPL6K9HQfEEGMGcToMGUrzNQxCzlg2g+"
    "Hg55i7iiKbA0ogENhEIFjMG+wmFDNzgjvDnNYOaPTQ7l4C8aaPsEfl3sugiw";

int cgi_klen() {
  return klen;
}

struct cgi_Cgi {
  char *fkey; // key to encrypt files
  char *key;  // key to communications. Must be set before to call ok or error
  char *home;
  time_t t_expiration;
};

static void write_users(Cgi *this, Achar *users);
static void put_user(Cgi *this, char *user, char *key, char *level);
static void write_sessions(Cgi *this, Arr/*Ajson*/ *sessions);

Cgi *cgi_new(char *home, time_t t_expiration) {
  rnd_init();

  Cgi *this = MALLOC(Cgi);

  this->fkey = cryp_key(deme_key, str_len(deme_key));
  this->key = NULL;
  this->home = home;
  this->t_expiration = t_expiration;

  if (!file_exists(home)) {
    file_mkdir(home);
  }

  char *fusers = path_cat(home, "users.db", NULL);
  if (!file_exists(fusers)) {
    write_users(this, achar_new());
    put_user(this, "admin", deme_key, "0");
    write_sessions(this, arr_new());
  }

  return this;
}

inline
char *cgi_home(Cgi *this) {
  return this->home;
}

// User ----------------------------------------------------

static void write_users(Cgi *this, Achar *users) {
  char *path = path_cat(this->home, "users.db", NULL);
  Ajson *tmp = ajson_new();
  EACH(users, char, udata) {
    ajson_add(tmp, json_wstring(udata));
  }_EACH
  file_write(path, cryp_cryp(this->fkey, (char *)json_warray(tmp)));
}

static Achar *read_users(Cgi *this) {
  char *path = path_cat(this->home, "users.db", NULL);
  Ajson *tmp = json_rarray((Json *)cryp_decryp(this->fkey, file_read(path)));
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

static void del_user(Cgi *this, char *user) {
  write_users(this, remove_user(read_users(this), user));
}

static void put_user(Cgi *this, char *user, char *key, char *level) {
  Achar *users = remove_user(read_users(this), user);
  char *kkey = cryp_key(key, klen);
  achar_add(users, str_printf("%s:%s:%s", user, kkey, level));
  write_users(this, users);
}

// If fails r is opt_null()
static Ochar *check_user(Cgi *this, char *id, char *key) {
  char *kkey = cryp_key(key, klen);
  Achar *users = read_users(this);

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

static bool change_level(Cgi *this, char *user, char *level) {
  Achar *users = read_users(this);

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
    write_users(this, users);
  }
  return r;
}

static bool change_pass(Cgi *this, char *user, char *old_pass, char *new_pass) {
  char *kold = cryp_key(old_pass, klen);
  char *knew = cryp_key(new_pass, klen);
  Achar *users = read_users(this);

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
    write_users(this, users);
  }
  return r;
}

// Session -------------------------------------------------

static void write_sessions(Cgi *this, Arr/*Ajson*/ *sessions) {
  char *path = path_cat(this->home, "sessions.db", NULL);
  Ajson *tmp = ajson_new();
  EACH(sessions, Ajson, sdata) {
    ajson_add(tmp, json_warray(sdata));
  }_EACH
  file_write(path, cryp_cryp(this->fkey, (char *)json_warray(tmp)));
}

static Arr/*Arr[Json]*/ *read_sessions(Cgi *this) {
  char *path = path_cat(this->home, "sessions.db", NULL);
  Ajson *tmp = json_rarray((Json*)cryp_decryp(this->fkey, file_read(path)));
  Arr/*char*/ *r = arr_new();
  EACH(tmp, Json, js) {
    arr_add(r, json_rarray(js));
  }_EACH
  return r;
}

// If expiration is 0 tNoExpiration is used
static void add_session(
  Cgi* this, char *session_id, char *user, char *key, time_t expiration
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
    this,
    arr_from_it(it_cat(
      it_unary(row),
      it_filter(arr_to_it(read_sessions(this)), filter)
    ))
  );
}

// In session.db:
// Fields are: sessionId:key:time:lapse
// If identification fails returns ""
static void read_session(
  char **key, char** conId, Cgi *this, char *session_id
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
    this,
    arr_from_it(it_map(it_filter(arr_to_it(read_sessions(this)), filter), map))
  );
}

static void del_session(Cgi *this, char *session_id) {
  /**/FNP(filter, Arr/*Json*/, row) {
  /**/  return !str_eq(session_id, json_rstring(arr_get(row, 0)));
  /**/}_FN
  write_sessions(
    this,
    arr_from_it(it_filter(arr_to_it(read_sessions(this)), filter))
  );
}

static void set_connection_id(Cgi *this, char *session_id, char *con_id) {
  /**/FNM(map, Arr/*Json*/, row) {
  /**/  if (str_eq(json_rstring(arr_get(row, 0)), session_id)){
  /**/    arr_set(row, 3, json_wstring(con_id));
  /**/  }
  /**/  return row;
  /**/}_FN
  write_sessions(
    this,
    arr_from_it(it_map(arr_to_it(read_sessions(this)), map))
  );
}

// Public interface ----------------------------------------

inline
void cgi_set_key(Cgi* this, char *k) {
  this->key = k;
}

void cgi_get_session_data(
  char **key, char **connectionId, Cgi *this, char *session_id
) {
  read_session(key, connectionId, this, session_id);
}

CgiRp *cgi_add_user(
  Cgi *this, char *admin, char *akey, char *user, char *ukey, char *level
){
  Mjson *m = mjson_new();
  Ochar *alevel = check_user(this, admin, akey);
  if (!ochar_is_null(alevel) && str_eq(ochar_value(alevel), "0")) {
    put_user(this, user, ukey, level);
    jmap_pbool(m, "ok", true);
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(this, m);
}

CgiRp *cgi_del_user(Cgi *this, char *admin, char *akey, char *user) {
  Mjson *m = mjson_new();
  Ochar *alevel = check_user(this, admin, akey);
  if (!ochar_is_null(alevel) && str_eq(ochar_value(alevel), "0")) {
    del_user(this, user);
    jmap_pbool(m, "ok", true);
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(this, m);
}

CgiRp *cgi_change_level(
  Cgi *this, char *admin, char *akey, char *user, char *level
) {
  Mjson *m = mjson_new();
  Ochar *alevel = check_user(this, admin, akey);
  if (!ochar_is_null(alevel) && str_eq(ochar_value(alevel), "0")) {
    if (change_level(this, user, level)) {
      jmap_pbool(m, "ok", true);
    } else {
      jmap_pbool(m, "ok", false);
    }
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(this, m);
}

CgiRp *cgi_change_pass(Cgi *this, char *user, char *key, char *new_key) {
  Mjson *m = mjson_new();
  if (change_pass(this, user, key, new_key)) {
    jmap_pbool(m, "ok", true);
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(this, m);
}


CgiRp *cgi_del_session(Cgi *this, char *session_id) {
  del_session(this, session_id);
  return cgi_ok(this, mjson_new());
}

CgiRp *cgi_authentication(Cgi *this, char *user, char *key, bool expiration) {
  Mjson *rp = mjson_new();

  Ochar *level = check_user(this, user, key);
  if (!ochar_is_null(level)) {
    char *session_id = cryp_genk(klen);
    char *key = cryp_genk(klen);
    add_session(
      this, session_id, user, key, expiration ? this->t_expiration : 0
    );
    jmap_pstring(rp, "level", ochar_value(level));
    jmap_pstring(rp, "sessionId", session_id);
    jmap_pstring(rp, "key", key);
  } else {
    jmap_pstring(rp, "level", "");
    jmap_pstring(rp, "sessionId", "");
    jmap_pstring(rp, "key", "");
  }

  return cgi_ok(this, rp);
}

CgiRp *cgi_connect(Cgi *this, char *session_id) {
  set_connection_id(this, session_id, cryp_genk(klen));

  char *key;
  char *connectionId;
  read_session(&key, &connectionId, this, session_id);

  Mjson *rp = mjson_new();
  jmap_pstring(rp, "key", key);
  jmap_pstring(rp, "connectionId", connectionId);
  return cgi_ok(this, rp);
}

CgiRp *cgi_ok(Cgi *this, Mjson *data) {
  if (!this->key) {
    return (CgiRp *)"cgi_ok: this->key not set";
  }

  jmap_pstring(data, "error", "");
  return (CgiRp *)cryp_cryp(this->key, (char *)json_wobject(data));
}

CgiRp *cgi_error(Cgi *this, char *msg) {
  if (!this->key) {
    return (CgiRp *)"cgi_ok: this->key not set";
  }

  Mjson *data = mjson_new();
  jmap_pstring(data, "error", msg);
  return (CgiRp *)cryp_cryp(this->key, (char *)json_wobject(data));
}

CgiRp *cgi_expired(Cgi *this) {
  Mjson *data = mjson_new();
  jmap_pbool(data, "expired", true);
  return cgi_ok(this, data);
}
