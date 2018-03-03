// Copyright 12-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <dmc/all.h>

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

static void write_users(Cgi *this, Arr/*char*/ *users);
static void put_user(Cgi *this, char *user, char *key, char *level);
static void write_sessions(Cgi *this, Arr/*char*/ *sessions);

Cgi *cgi_new(char *home, time_t t_expiration) {
  rnd_init();

  Cgi *this = MALLOC(Cgi);

	this->fkey = cryp_key(deme_key, strlen(deme_key));
  this->key = NULL;
  this->home = home;
  this->t_expiration = t_expiration;

	if (!file_exists(home)) {
		file_mkdir(home);
  }

  char *fusers = path_cat(home, "users.db", NULL);
  if (!file_exists(fusers)) {
    Arr/*char*/ *a = arr_new();
    write_users(this, a);
    put_user(this, "admin", deme_key, "0");
    write_sessions(this, a);
  }

  return this;
}

inline
char *cgi_home(Cgi *this) {
  return this->home;
}

// User ----------------------------------------------------

static void write_users(Cgi *this, Arr/*char*/ *users) {
  char *path = path_cat(this->home, "users.db", NULL);
  Arr/*Json*/ *tmp = arr_new();
  EACH(users, char, udata) {
    arr_add(tmp, json_wstring(udata));
  }_EACH
  file_write(path, cryp_cryp(this->fkey, json_warray(tmp)));
}

static Arr/*char*/ *read_users(Cgi *this) {
  char *path = path_cat(this->home, "users.db", NULL);
  Arr/*Json*/ *tmp = json_rarray(cryp_decryp(this->fkey, file_read(path)));
  Arr/*char*/ *r = arr_new();
  EACH(tmp, Json, js) {
    arr_add(r, json_rstring(js));
  }_EACH
  return r;
}

static Arr/*char*/ *remove_user(Arr/*char*/ *users, char *user) {
  Arr/*char*/ *r = arr_new();
  EACH(users, char, udata) {
    Arr/*char*/ *parts = str_csplit(udata, ':');
    if (strcmp(user, arr_get(parts, 0))) {
      arr_add(r, udata);
    }
  }_EACH
  return r;
}

static void del_user(Cgi *this, char *user) {
	write_users(this, remove_user(read_users(this), user));
}

static void put_user(Cgi *this, char *user, char *key, char *level) {
	Arr/*char*/ *users = remove_user(read_users(this), user);
  char *kkey = cryp_key(key, klen);
  arr_add(users, str_printf("%s:%s:%s", user, kkey, level));
	write_users(this, users);
}

// If fails r is NULL
static char *check_user(Cgi *this, char *id, char *key) {
  char *kkey = cryp_key(key, klen);
  Arr/*char*/ *users = read_users(this);

  EACH(users, char, udata) {
    Arr/*char*/ *parts = str_csplit(udata, ':');
    if (
      !strcmp(arr_get(parts, 0), id) &&
      !strcmp(arr_get(parts, 1), kkey)
    ) {
      return arr_get(parts, 2);
    }
  }_EACH
  return NULL;
}

static bool change_level(Cgi *this, char *user, char *level) {
  Arr/*char*/ *users = read_users(this);

  bool r = false;
  RANGE0(i, arr_size(users)){
    char *udata = arr_get(users, i);
    Arr/*char*/ *parts = str_csplit(udata, ':');
    if (!strcmp(arr_get(parts, 0), user)) {
      char *new_udata = str_printf("%s:%s:%s",
        (char *)arr_get(parts, 0), (char *)arr_get(parts, 1), level
      );
      arr_set(users, i, new_udata);
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
  Arr/*char*/ *users = read_users(this);

  bool r = false;
  RANGE0(i, arr_size(users)){
    char *udata = arr_get(users, i);
    Arr/*char*/ *parts = str_csplit(udata, ':');
    if (
      !strcmp(arr_get(parts, 0), user) &&
      !strcmp(arr_get(parts, 1), kold)
    ) {
      char *new_udata = str_printf("%s:%s:%s",
        (char *)arr_get(parts, 0), knew, (char *)arr_get(parts, 2)
      );
      arr_set(users, i, new_udata);
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

static void write_sessions(Cgi *this, Arr/*Arr[Json]*/ *sessions) {
  char *path = path_cat(this->home, "sessions.db", NULL);
  Arr/*Json*/ *tmp = arr_new();
  EACH(sessions, Arr/*Json*/, sdata) {
    arr_add(tmp, json_warray(sdata));
  }_EACH
  file_write(path, cryp_cryp(this->fkey, json_warray(tmp)));
}

static Arr/*Arr[Json]*/ *read_sessions(Cgi *this) {
  char *path = path_cat(this->home, "sessions.db", NULL);
  Arr/*Json*/ *tmp = json_rarray(cryp_decryp(this->fkey, file_read(path)));
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

  Arr/*Json*/ *row = arr_new();
  jarr_astring(row, session_id);  //0
  jarr_astring(row, user);       //1
  jarr_astring(row, key);        //2
  jarr_astring(row, "");         //3
  jarr_auint(row, time);         //4
  jarr_auint(row, lapse);        //5

  /**/FNP(filter, Arr/*Json*/, row) {
  /**/  return strcmp(user, json_rstring(arr_get(row, 1)));
  /**/}_FN
  write_sessions(
    this,
    it_to(it_cat(
      it_unary(row),
      it_filter(it_from(read_sessions(this)), filter)
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
  /**/  if (!strcmp(json_rstring(arr_get(row, 0)), session_id)){
  /**/    *key = json_rstring(arr_get(row, 2));
  /**/    *conId = json_rstring(arr_get(row, 3));
  /**/  }
  /**/  arr_set(row, 4, json_wuint(now + json_ruint(arr_get(row, 5))));
  /**/  return row;
  /**/}_FN
  write_sessions(
    this,
    it_to(it_map(it_filter(it_from(read_sessions(this)), filter), map))
  );
}

static void del_session(Cgi *this, char *session_id) {
  /**/FNP(filter, Arr/*Json*/, row) {
  /**/  return strcmp(session_id, json_rstring(arr_get(row, 0)));
  /**/}_FN
  write_sessions(
    this,
    it_to(it_filter(it_from(read_sessions(this)), filter))
  );
}

static void set_connection_id(Cgi *this, char *session_id, char *con_id) {
  /**/FNM(map, Arr/*Json*/, row) {
  /**/  if (!strcmp(json_rstring(arr_get(row, 0)), session_id)){
  /**/    arr_set(row, 3, json_wstring(con_id));
  /**/  }
  /**/  return row;
  /**/}_FN
  write_sessions(
    this,
    it_to(it_map(it_from(read_sessions(this)), map))
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
  Map/*Json*/ *m = map_new();
  char *alevel = check_user(this, admin, akey);
  if (alevel && !strcmp(alevel, "0")) {
    put_user(this, user, ukey, level);
    jmap_pbool(m, "ok", true);
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(this, m);
}

CgiRp *cgi_del_user(Cgi *this, char *admin, char *akey, char *user) {
  Map/*Json*/ *m = map_new();
  char *alevel = check_user(this, admin, akey);
  if (alevel && !strcmp(alevel, "0")) {
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
  Map/*Json*/ *m = map_new();
  char *alevel = check_user(this, admin, akey);
  if (alevel && !strcmp(alevel, "0")) {
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
  Map/*Json*/ *m = map_new();
  if (change_pass(this, user, key, new_key)) {
    jmap_pbool(m, "ok", true);
  } else {
    jmap_pbool(m, "ok", false);
  }
  return cgi_ok(this, m);
}


CgiRp *cgi_del_session(Cgi *this, char *session_id) {
  del_session(this, session_id);
  return cgi_ok(this, map_new());
}

CgiRp *cgi_authentication(Cgi *this, char *user, char *key, bool expiration) {
	Map/*Json*/ *rp = map_new();

  char *level = check_user(this, user, key);
  if (level) {
    char *session_id = cryp_genk(klen);
    char *key = cryp_genk(klen);
    add_session(
      this, session_id, user, key, expiration ? this->t_expiration : 0
    );
    jmap_pstring(rp, "level", level);
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

	Map/*Json*/ *rp = map_new();
  jmap_pstring(rp, "key", key);
  jmap_pstring(rp, "connectionId", connectionId);
  return cgi_ok(this, rp);
}

CgiRp *cgi_ok(Cgi *this, Map/*Json*/ *data) {
  if (!this->key) {
    return "cgi_ok: this->key not set";
  }

  jmap_pstring(data, "error", "");
  return cryp_cryp(this->key, json_wobject(data));
}

CgiRp *cgi_error(Cgi *this, char *msg) {
  if (!this->key) {
    return "cgi_ok: this->key not set";
  }

  Map/*Json*/ *data = map_new();
  jmap_pstring(data, "error", msg);
  return cryp_cryp(this->key, json_wobject(data));
}

CgiRp *cgi_expired(Cgi *this) {
  Map/*Json*/ *data = map_new();
  jmap_pbool(data, "expired", true);
  return cgi_ok(this, data);
}
