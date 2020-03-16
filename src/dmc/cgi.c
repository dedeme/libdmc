// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/cgi.h"
#include "dmc/rnd.h"
#include "dmc/cryp.h"
#include "dmc/date.h"
#include "dmc/async.h"

static size_t klen = 300;
static time_t t_no_expiration = 2592000; // seconds == 30 days
static char *deme_key =
    "nkXliX8lg2kTuQSS/OoLXCk8eS4Fwmc+N7l6TTNgzM1vdKewO0cjok51vcdl"
    "OKVXyPu83xYhX6mDeDyzapxL3dIZuzwyemVw+uCNCZ01WDw82oninzp88Hef"
    "bn3pPnSMqEaP2bOdX+8yEe6sGkc3IO3e38+CqSOyDBxHCqfrZT2Sqn6SHWhR"
    "KqpJp4K96QqtVjmXwhVcST9l+u1XUPL6K9HQfEEGMGcToMGUrzNQxCzlg2g+"
    "Hg55i7iiKbA0ogENhEIFjMG+wmFDNzgjvDnNYOaPTQ7l4C8aaPsEfl3sugiw";

struct cgi_Session {
  char *user;
  char *level;
  char *com_key;
};

static CgiSession *cgiSession_new(
  char *user, char *level, char *com_key
) {
  CgiSession *this = MALLOC(CgiSession);
  this->user = user;
  this->level = level;
  this->com_key = com_key;
  return this;
}

char *cgiSession_user(CgiSession *this) {
  return this->user;
}

char *cgiSession_level(CgiSession *this) {
  return this->level;
}

char *cgiSession_key(CgiSession *this) {
  return this->com_key;
}

struct cgi_Cgi {
  char *fkey;      // key to encrypt files
  // Opt[char]
  Opt *key;        // key to communications.
  char *home;
  time_t t_expiration;
};

typedef struct cgi_Cgi Cgi;

static Cgi *cgi_null = NULL;

// users is Arr[char]
static void write_users(Arr *users);
static void put_user(char *user, char *key, char *level);
// sessions is Arr[Js]
static void write_sessions(Arr *sessions);


void cgi_init(char *home, time_t t_expiration) {
  rnd_init();

  cgi_null = MALLOC(Cgi);

  cgi_null->fkey = cryp_key(deme_key, strlen(deme_key));
  cgi_null->key = opt_empty();
  cgi_null->home = str_new(home);
  cgi_null->t_expiration = t_expiration;

  if (!file_exists(home)) {
    file_mkdir(home);
  }

  char *fusers = path_cat(home, "users.db", NULL);
  if (!file_exists(fusers)) {
    // Arr[char]
    Arr *a = arr_new();
    write_users(a);
    put_user("admin", deme_key, "0");
    write_sessions(a);
  }
}

// User ----------------------------------------------------

// users is Arr[char]
static void write_users(Arr *users) {
  char *path = path_cat(cgi_null->home, "users.db", NULL);
  // Arr[Js]
  Arr *tmp = arr_from_it(it_map(arr_to_it(users), (FCOPY)js_ws));
  char *js = (char *)js_wa(tmp);
  js = cryp_cryp(js, cgi_null->fkey);

  file_write(path, js);
}

// Returns Arr[char]
static Arr *read_users(void) {
  char *path = path_cat(cgi_null->home, "users.db", NULL);
  char *tx = file_read(path);
  tx = cryp_decryp(tx, cgi_null->fkey);

  return arr_from_it(it_map(arr_to_it(js_ra((Js *)tx)), (FCOPY)js_rs));
}

// Users is Arr[char]
static void remove_user(Arr *users, char *user) {
  // ------------------------------------------------------------------------ //
  int pred(char *row) {                                                       //
    // Arr[char]                                                              //
    Arr *parts = str_csplit(row, ':');                                        //
    int r = str_eq(user, arr_get(parts, 0));                                  //
    return r;                                                                 //
  }                                                                           //
  // ------------------------------------------------------------------------ //
  int ix = arr_index(users, (FPRED)pred);
  if (ix != -1) {
    arr_remove(users, ix);
  }
}

static void del_user(char *user) {
  // Arr[char]
  Arr *users = read_users();
  remove_user(users, user);
  write_users(users);
}


static void put_user(char *user, char *key, char *level) {
  // Arr[char]
  Arr *users = read_users();
  remove_user(users, user);
  char *kkey = cryp_key(key, klen);
  arr_push(
    users,
    str_cat(user, ":", kkey, ":", level, NULL)
  );
  write_users(users);
}

// Returns level of "id" (Opt[char])
static Opt *check_user(char *id, char *key) {
  char *kkey = cryp_key(key, klen);

  // Arr[char]
  Arr *users = read_users();

  Opt *r = opt_empty();
  EACH(users, char, udata) {
    // Arr[char]
    Arr *parts = str_csplit(udata, ':');
    if (
      str_eq(arr_get(parts, 0), id) &&
      str_eq(arr_get(parts, 1), kkey)
    ) {
      r = opt_new(arr_get(parts, 2));
      break;
    }
  }_EACH

  return r;
}

static int change_level(char *user, char *level) {
  // ------------------------------------------------------------------------ //
  int pred(char *row) {                                                       //
    // Arr[char]                                                              //
    Arr *parts = str_csplit(row, ':');                                        //
    return str_eq(user, arr_get(parts, 0));                                   //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  // Arr[char]
  Arr *users = read_users();
  int ix = arr_index(users, (FPRED)pred);

  if (ix == -1) {
    return 0;
  }

  char *row = arr_get(users, ix);
  // Arr[char]
  Arr *parts = str_csplit(row, ':');
  char *new_row = str_cat(
    arr_get(parts, 0), ":",
    arr_get(parts, 1), ":",
    level, NULL
  );
  arr_set(users, ix, new_row);
  write_users(users);

  return 1;
}

static int change_pass(char *user, char *old_pass, char *new_pass) {
  // ------------------------------------------------------------------------ //
  int pred(char *row) {                                                       //
    char *kold = cryp_key(old_pass, klen);                                    //
    // Arr[char]                                                              //
    Arr *parts = str_csplit(row, ':');                                        //
    return str_eq(arr_get(parts, 0), user) &&                                 //
            str_eq(arr_get(parts, 1), kold);                                  //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  // Arr[char]
  Arr *users = read_users();
  int ix = arr_index(users, (FPRED)pred);

  if (ix == -1) {
    return 0;
  }

  char *knew = cryp_key(new_pass, klen);
  char *row = arr_get(users, ix);
  // Arr[char]
  Arr *parts = str_csplit(row, ':');
  char *new_row = str_cat(
    arr_get(parts, 0), ":",
    knew, ":",
    arr_get(parts, 2), NULL
  );
  arr_set(users, ix, new_row);
  write_users(users);

  return 1;
}

// Session -------------------------------------------------

// sessions is Arr[Js] (Where Js is a json array)
static void write_sessions(Arr *sessions) {
  char *path = path_cat(cgi_null->home, "sessions.db", NULL);
  char *data = (char *)js_wa(sessions);
  data = cryp_cryp(data, cgi_null->fkey);
  file_write(path, data);
}

// Returns Arr[Js] (Where Js is a json array)
static Arr *read_sessions_new(void) {
  char *path = path_cat(cgi_null->home, "sessions.db", NULL);
  char *data = file_read(path);
  data = cryp_decryp(data, cgi_null->fkey);
  return js_ra((Js*)data);
}

// If expiration is 0 tNoExpiration is used
static void add_session(
  char *session_id, char *user, char *key, char *level, time_t expiration
) {
  // Arr[Js] (Where Js is a json array)
  Arr *sessions = read_sessions_new();

  time_t lapse = expiration ? expiration : t_no_expiration;
  time_t time = date_now() + lapse;

  // Arr[Js]
  Arr *new_row = arr_new();
  arr_push(new_row,  js_ws(session_id));  //0
  arr_push(new_row,  js_ws(user));        //1
  arr_push(new_row,  js_ws(key));         //2
  arr_push(new_row,  js_ws(level));       //3
  arr_push(new_row,  date_to_js(time));   //4
  arr_push(new_row,  date_to_js(lapse));  //5

  arr_push(sessions, js_wa(new_row));
  write_sessions(sessions);
}

// In session.db:
// Fields are: sessionId:key:time:lapse
// Return is Opt[CgiSession]
static Opt *read_session(char *session_id) {
  time_t now = date_now();

  // ------------------------------------------------------------------------ //
  // jsrow is a Js(Arr[Js])                                                   //
  int filter(Js *jsrow) {                                                     //
    // Arr[Js]                                                                //
    Arr *row = js_ra(jsrow);                                                  //
    return (date_from_js(arr_get(row, 4)) >= now);                            //
  }                                                                           //
                                                                              //
  // jsrow is a Js(Arr[Js])                                                   //
  int pred(Js *jsrow) {                                                       //
    // Arr[js]                                                                //
    Arr *row = js_ra(jsrow);                                                  //
    char *s_id = js_rs(arr_get(row, 0));                                      //
    return str_eq(s_id, session_id);                                          //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  // Arr[Js] (Where Js is a json array)
  Arr *sessions = read_sessions_new();

  arr_filter_in(sessions, (FPRED)filter);

  int ix = arr_index(sessions, (FPRED)pred);
  // Opt[CgiSession]
  Opt *r = opt_empty();
  if (ix != -1) {
    // Arr[js]
    Arr *row = js_ra(arr_get(sessions, ix));
    char *user = js_rs(arr_get(row, 1));
    char *com_key = js_rs(arr_get(row, 2));
    char *level = js_rs(arr_get(row, 3));
    r = opt_new(cgiSession_new(user, level, com_key));

    arr_set(row, 4, date_to_js(
      date_now() + date_from_js(arr_get(row, 5))
    ));
    arr_set(sessions, ix, js_wa(row));
  }

  write_sessions(sessions);
  return r;
}

static void del_session(char *session_id) {
  // ------------------------------------------------------------------------ //
  // jsrow is a Js(Arr[Js])                                                   //
  int pred(Js *jsrow) {                                                       //
    // Arr[js]                                                                //
    Arr *row = js_ra(jsrow);                                                  //
    char *s_id = js_rs(arr_get(row, 0));                                      //
    return str_eq(s_id, session_id);                                          //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  // Arr[Js] (Where Js is a json array)
  Arr *sessions = read_sessions_new();
  int ix = arr_index(sessions, (FPRED)pred);

  if (ix != -1) {
    arr_remove(sessions, ix);
    write_sessions(sessions);
  }
}

// Public interface ----------------------------------------

int cgi_klen(void) {
  return klen;
}

char *cgi_home(void) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  return cgi_null->home;
}

void cgi_set_key(char *k) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  cgi_null->key = opt_new(k);
}

// Returns Opt[CgiSession]
Opt *cgi_get_session(char *session_id) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  return read_session(session_id);
}

char *cgi_add_user(
  char *admin, char *akey,
  char *user, char *ukey, char *level
){
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  // Map[Js]
  Map *m = map_new();
  // Opt[char]
  Opt *alevel = check_user(admin, akey);
  if (opt_is_full(alevel) && str_eq(opt_get(alevel), "0")) {
    put_user(user, ukey, level);
    map_put(m, "ok", js_wb(1));
  } else {
    map_put(m, "ok", js_wb(0));
  }
  return cgi_ok(m);
}

char *cgi_del_user(char *admin, char *akey, char *user) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  // Map[Js]
  Map *m = map_new();
  // Opt[char]
  Opt *alevel = check_user(admin, akey);
  if (opt_is_full(alevel) && str_eq(opt_get(alevel), "0")) {
    del_user(user);
    map_put(m, "ok", js_wb(1));
  } else {
    map_put(m, "ok", js_wb(0));
  }
  return cgi_ok(m);
}

char *cgi_change_level(
  char *admin, char *akey, char *user, char *level
) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  // Map[Js]
  Map *m = map_new();
  // Opt[char]
  Opt *alevel = check_user(admin, akey);
  if (opt_is_full(alevel) && str_eq(opt_get(alevel), "0")) {
    if (change_level(user, level)) {
      map_put(m, "ok", js_wb(1));
    } else {
      map_put(m, "ok", js_wb(0));
    }
  } else {
    map_put(m, "ok", js_wb(0));
  }
  return cgi_ok(m);
}

char *cgi_change_pass(char *user, char *key, char *new_key) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  // Map[Js]
  Map *m = map_new();
  if (change_pass(user, key, new_key)) {
    map_put(m, "ok", js_wb(1));
  } else {
    map_put(m, "ok", js_wb(0));
  }
  return cgi_ok(m);
}

char *cgi_del_session(char *session_id) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  del_session(session_id);
  return cgi_empty();
}

char *cgi_authentication(char *user, char *key, int expiration) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  // Map[Js]
  Map *m = map_new();

  // Opt[char]
  char *level = opt_nget(check_user(user, key));
  if (level) {
    char *session_id = cryp_genk(klen);
    char *key = cryp_genk(klen);
    add_session(
      session_id, user, key, level, expiration ? cgi_null->t_expiration : 0
    );
    map_put(m, "level", js_ws(level));
    map_put(m, "sessionId", js_ws(session_id));
    map_put(m, "key", js_ws(key));
  } else {
    map_put(m, "level", js_ws(""));
    map_put(m, "sessionId", js_ws(""));
    map_put(m, "key", js_ws(""));
  }

  return cgi_ok(m);
}

char *cgi_connect(char *session_id) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  // Map[Js]
  Map *m = map_new();

  char *com_key = "";
  char *user = "";
  char *level = "";
  // Opt[CgiSession]
  Opt *cgiss = read_session(session_id);
  if (opt_is_full(cgiss)) {
    CgiSession *ss = opt_get(cgiss);
    com_key = ss->com_key;
    user = ss->user;
    level = ss->level;
  }

  map_put(m, "key", js_ws(com_key));
  map_put(m, "user", js_ws(user));
  map_put(m, "level", js_ws(level));
  return cgi_ok(m);
}

// data is Map[Js]
char *cgi_ok(Map *data) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  char *msg = (char *)js_wo(data);
  return cryp_cryp(
    msg,
    opt_eget(cgi_null->key, "Cgi->key is not set")
  );

/*
  // To debug
  FILE *tmp = fopen("data/tmp.txt", "w");
  fputs(msg, tmp);
  fclose(tmp);


//  fputs(msg, stdout);
*/
}

char *cgi_empty(void) {
  return cgi_ok(map_new());
}

char *cgi_error(char *msg) {
  // Map[Js]
  Map *m = map_new();
  map_put(m, "error", js_ws(msg));
  return cgi_ok(m);
}

char *cgi_expired(void) {
  // Map[Js]
  Map *m = map_new();
  map_put(m, "expired", js_wb(1));
  return cgi_ok(m);
}

// fn_data is Tp3[Map[Js] *(*fn)(void *ctx, Map[Js] *rq), void, Map[Js]]
static void long_run(Tp3 *fn_v_rq) {
  Map *(*lrun)(void *, Map *) = tp3_e1(fn_v_rq);
  void *ctx = tp3_e2(fn_v_rq);
  Map *rq = tp3_e3(fn_v_rq);
  CGI_GET_STR(longRunFile, rq)

  // Map[js]
  Map *rp = lrun(ctx, rq);
  map_put(rp, "longRunEnd", js_wb(1));

  char *tmp = file_tmp("dmc_cgi_long_run");
  file_write(tmp, (char *)js_wo(rp));
  file_rename(tmp, longRunFile);
}

// All Maps are Map[Js]
Map *cgi_long_run(Map *(*fn)(void *ctx, Map *rq), void *ctx, Map *rq) {
  CGI_GET_STR(longRunFile, rq)
  // Map[Js]
  Map *rp = map_new();
  if (*longRunFile) {
    if (file_exists(longRunFile)) {
      rp = js_ro((Js *)file_read(longRunFile));
      map_put(rp, "longRunEnd", js_wb(1));
      file_del(longRunFile);
    } else {
      map_put(rp, "longRunEnd", js_wb(0));
    }
  } else {
    Js *long_run_file = js_ws(file_tmp("dmc_cgi_long_run"));
    map_put(rq, "longRunFile", long_run_file);
    map_put(rp, "longRunFile", long_run_file);
    async_thread_detached((FPROC)long_run, tp3_new(fn, ctx, rq));
  }
  return rp;
}
