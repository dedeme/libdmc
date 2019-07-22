// Copyright 22-Jul-2019 ºDeme
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
  char *com_key;
  char *con_id;
};

static CgiSession *cgiSession_new(Gc *gc, char *com_key, char *con_id) {
  CgiSession *this = gc_add(gc, malloc(sizeof(CgiSession)));
  this->com_key = com_key;
  this->con_id = con_id;
  return this;
}

char *cgiSession_key(CgiSession *this) {
  return this->com_key;
}

char *cgiSession_id(CgiSession *this) {
  return this->con_id;
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


void cgi_init(Gc *gc, char *home, time_t t_expiration) {
  if (cgi_null) return;

  rnd_init();

  cgi_null = gc_add(gc, malloc(sizeof(Cgi)));

  cgi_null->fkey = cryp_key(gc, deme_key, strlen(deme_key));
  cgi_null->key = opt_empty();
  cgi_null->home = str_new(gc, home);
  cgi_null->t_expiration = t_expiration;

  if (!file_exists(home)) {
    file_mkdir(home);
  }

  Gc *gcl = gc_new();
  char *fusers = path_cat(gcl, home, "users.db", NULL);
  if (!file_exists(fusers)) {
    // Arr[char]
    Arr *a = arr_new(gcl);
    write_users(a);
    put_user("admin", deme_key, "0");
    write_sessions(a);
  }
  gc_free(gcl);
}

// User ----------------------------------------------------

// users is Arr[char]
static void write_users(Arr *users) {
  Gc *gc = gc_new();
  char *path = path_cat(gc, cgi_null->home, "users.db", NULL);
  // Arr[Js]
  Arr *tmp = arr_from_it(gc, it_map(gc, arr_to_it(gc, users), (FCOPY)js_ws));
  char *js = (char *)js_wa(gc, tmp);
  js = cryp_cryp(gc, js, cgi_null->fkey);

  file_write(path, js);
  gc_free(gc);
}

// Returns Arr[char]
static Arr *read_users(Gc *gc) {
  Gc *gcl = gc_new();
  char *path = path_cat(gcl, cgi_null->home, "users.db", NULL);
  char *tx = file_read(gcl, path);
  tx = cryp_decryp(gcl, tx, cgi_null->fkey);

  // Arr[char]
  Arr *r = arr_new(gc);
  EACH(js_ra(gcl, (Js *)tx), Js, js)
    arr_push(r, js_rs(gc, js));
  _EACH

  gc_free(gcl);
  return r;
}

// Users is Arr[char]
static void remove_user(Arr *users, char *user) {
  // ------------------------------------------------------------------------ //
  int pred(char *row) {                                                       //
    Gc *gc = gc_new();                                                                   //
    // Arr[char]                                                              //
    Arr *parts = str_csplit(gc, row, ':');                                    //
    int r = str_eq(user, arr_get(parts, 0));                                  //
    gc_free(gc);                                                              //
    return r;                                                                 //
  }                                                                           //
  // ------------------------------------------------------------------------ //
  int ix = arr_index(users, (FPRED)pred);
  if (ix != -1) {
    arr_remove(users, ix);
  }
}

static void del_user(char *user) {
  Gc *gc = gc_new();
  // Arr[char]
  Arr *users = read_users(gc);
  remove_user(users, user);
  write_users(users);
  gc_free(gc);
}


static void put_user(char *user, char *key, char *level) {
  Gc *gc = gc_new();
  // Arr[char]
  Arr *users = read_users(gc);
  remove_user(users, user);
  char *kkey = cryp_key(gc, key, klen);
  arr_push(
    users,
    str_cat(gc, user, ":", kkey, ":", level, NULL)
  );
  write_users(users);
  gc_free(gc);
}

// Returns level of "id" (Opt[char])
static Opt *check_user(Gc *gc, char *id, char *key) {
  Gc *gcl = gc_new();
  char *kkey = cryp_key(gcl, key, klen);

  // Arr[char]
  Arr *users = read_users(gcl);

  Opt *r = opt_empty();
  EACH(users, char, udata) {
    // Arr[char]
    Arr *parts = str_csplit(gcl, udata, ':');
    if (
      str_eq(arr_get(parts, 0), id) &&
      str_eq(arr_get(parts, 1), kkey)
    ) {
      r = opt_new(str_new(gc, arr_get(parts, 2)));
      break;
    }
  }_EACH

  return r;
}

static int change_level(char *user, char *level) {
  // ------------------------------------------------------------------------ //
  int pred(char *row) {                                                       //
    Gc *gc = gc_new();                                                        //
    // Arr[char]                                                              //
    Arr *parts = str_csplit(gc, row, ':');                                    //
    int r = str_eq(user, arr_get(parts, 0));                                  //
    gc_free(gc);                                                              //
    return r;                                                                 //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  Gc *gc = gc_new();
  // Arr[char]
  Arr *users = read_users(gc);
  int ix = arr_index(users, (FPRED)pred);

  if (ix == -1) {
    gc_free(gc);
    return 0;
  }

  char *row = arr_get(users, ix);
  // Arr[char]
  Arr *parts = str_csplit(gc, row, ':');
  char *new_row = str_cat(gc,
    arr_get(parts, 0), ":",
    arr_get(parts, 1), ":",
    level, NULL
  );
  arr_set(users, ix, new_row);
  write_users(users);

  gc_free(gc);
  return 1;
}

static int change_pass(char *user, char *old_pass, char *new_pass) {
  // ------------------------------------------------------------------------ //
  int pred(char *row) {                                                       //
    Gc *gc = gc_new();                                                        //
    char *kold = cryp_key(gc, old_pass, klen);                                //
    // Arr[char]                                                              //
    Arr *parts = str_csplit(gc, row, ':');                                    //
    int r = str_eq(arr_get(parts, 0), user) &&                                //
            str_eq(arr_get(parts, 1), kold);                                  //
    gc_free(gc);                                                              //
    return r;                                                                 //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  Gc *gc = gc_new();
  // Arr[char]
  Arr *users = read_users(gc);
  int ix = arr_index(users, (FPRED)pred);

  if (ix == -1) {
    gc_free(gc);
    return 0;
  }

  char *knew = cryp_key(gc, new_pass, klen);
  char *row = arr_get(users, ix);
  // Arr[char]
  Arr *parts = str_csplit(gc, row, ':');
  char *new_row = str_cat(gc,
    arr_get(parts, 0), ":",
    knew, ":",
    arr_get(parts, 2), NULL
  );
  arr_set(users, ix, new_row);
  write_users(users);

  gc_free(gc);
  return 1;
}

// Session -------------------------------------------------

// sessions is Arr[Js] (Where Js is a json array)
static void write_sessions(Arr *sessions) {
  Gc *gc = gc_new();
  char *path = path_cat(gc, cgi_null->home, "sessions.db", NULL);
  char *data = (char *)js_wa(gc, sessions);
  data = cryp_cryp(gc, data, cgi_null->fkey);
  file_write(path, data);
  gc_free(gc);
}

// Returns Arr[Js] (Where Js is a json array)
static Arr *read_sessions_new(Gc *gc) {
  Gc *gcl = gc_new();
  char *path = path_cat(gcl, cgi_null->home, "sessions.db", NULL);
  char *data = file_read(gcl, path);
  data = cryp_decryp(gcl, data, cgi_null->fkey);
  // Arr[Js]
  Arr *r = js_ra(gc, (Js*)data);
  gc_free(gcl);
  return r;
}

// If expiration is 0 tNoExpiration is used
static void add_session(
  char *session_id, char *user, char *key, time_t expiration
) {
  Gc *gc = gc_new();
  // Arr[Js] (Where Js is a json array)
  Arr *sessions = read_sessions_new(gc);

  time_t lapse = expiration ? expiration : t_no_expiration;
  time_t time = date_now() + lapse;

  // Arr[Js]
  Arr *new_row = arr_new(gc);
  arr_push(new_row,  js_ws(gc, session_id));  //0
  arr_push(new_row,  js_ws(gc, user));        //1
  arr_push(new_row,  js_ws(gc, key));         //2
  arr_push(new_row,  js_ws(gc, ""));          //3
  arr_push(new_row,  date_to_js(gc, time));   //4
  arr_push(new_row,  date_to_js(gc, lapse));  //5

  arr_push(sessions, js_wa(gc, new_row));
  write_sessions(sessions);
}

// In session.db:
// Fields are: sessionId:key:time:lapse
// Return is Opt[CgiSession]
static Opt *read_session(Gc *gc, char *session_id) {
  time_t now = date_now();

  // ------------------------------------------------------------------------ //
  // jsrow is a Js(Arr[Js])                                                   //
  int filter(Js *jsrow) {                                                     //
    Gc *gc = gc_new();                                                        //
    // Arr[Js]                                                                //
    Arr *row = js_ra(gc, jsrow);                                              //
    int r = (date_from_js(gc, arr_get(row, 4)) >= now);                       //
    gc_free(gc);                                                              //
    return r;                                                                 //
  }                                                                           //
                                                                              //
  // jsrow is a Js(Arr[Js])                                                   //
  int pred(Js *jsrow) {                                                       //
    Gc *gc = gc_new();                                                        //
    // Arr[js]                                                                //
    Arr *row = js_ra(gc, jsrow);                                              //
    char *s_id = js_rs(gc, arr_get(row, 0));                                  //
    int r = str_eq(s_id, session_id);                                         //
    gc_free(gc);                                                              //
    return r;                                                                 //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  Gc *gcl = gc_new();
  // Arr[Js] (Where Js is a json array)
  Arr *sessions = read_sessions_new(gcl);

  arr_filter(sessions, (FPRED)filter);

  int ix = arr_index(sessions, (FPRED)pred);
  // Opt[CgiSession]
  Opt *r = opt_empty();
  if (ix != -1) {
    // Arr[js]
    Arr *row = js_ra(gcl, arr_get(sessions, ix));
    char *com_key = js_rs(gc, arr_get(row, 2));
    char *con_id = js_rs(gc, arr_get(row, 3));
    r = opt_new(cgiSession_new(gc, com_key, con_id));

    arr_set(row, 4, date_to_js(
      gcl, date_now() + date_from_js(gcl, arr_get(row, 5))
    ));
    arr_set(sessions, ix, js_wa(gcl, row));
  }

  write_sessions(sessions);
  gc_free(gcl);
  return r;
}

static void del_session(char *session_id) {
  // ------------------------------------------------------------------------ //
  // jsrow is a Js(Arr[Js])                                                   //
  int pred(Js *jsrow) {                                                       //
    Gc *gc = gc_new();                                                        //
    // Arr[js]                                                                //
    Arr *row = js_ra(gc, jsrow);                                              //
    char *s_id = js_rs(gc, arr_get(row, 0));                                  //
    int r = str_eq(s_id, session_id);                                         //
    gc_free(gc);                                                              //
    return r;                                                                 //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  Gc *gc = gc_new();
  // Arr[Js] (Where Js is a json array)
  Arr *sessions = read_sessions_new(gc);
  int ix = arr_index(sessions, (FPRED)pred);

  if (ix != -1) {
    arr_remove(sessions, ix);
    write_sessions(sessions);
  }
  gc_free(gc);
}

static void set_connection_id(char *session_id, char *con_id) {
  // ------------------------------------------------------------------------ //
  // jsrow is a Js(Arr[Js])                                                   //
  int pred(Js *jsrow) {                                                       //
    Gc *gc = gc_new();                                                        //
    // Arr[js]                                                                //
    Arr *row = js_ra(gc, jsrow);                                              //
    char *s_id = js_rs(gc, arr_get(row, 0));                                  //
    int r = str_eq(s_id, session_id);                                         //
    gc_free(gc);                                                              //
    return r;                                                                 //
  }                                                                           //
  // ------------------------------------------------------------------------ //

  Gc *gc = gc_new();
  // Arr[Js] (Where Js is a json array)
  Arr *sessions = read_sessions_new(gc);
  int ix = arr_index(sessions, (FPRED)pred);

  if (ix != -1) {
    // Arr[js]
    Arr *row = js_ra(gc, arr_get(sessions, ix));
    arr_set(row, 3, js_ws(gc, con_id));
    arr_set(sessions, ix, js_wa(gc, row));
    write_sessions(sessions);
  }
  gc_free(gc);
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
Opt *cgi_get_session(Gc *gc, char *session_id) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  return read_session(gc, session_id);
}

char *cgi_add_user(
  Gc *gc,
  char *admin, char *akey,
  char *user, char *ukey, char *level
){
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  Gc *gcl = gc_new();
  // Map[Js]
  Map *m = map_new(gcl);
  // Opt[char]
  Opt *alevel = check_user(gcl, admin, akey);
  if (opt_is_full(alevel) && str_eq(opt_get(alevel), "0")) {
    put_user(user, ukey, level);
    map_put(m, "ok", js_wb(gcl, 1));
  } else {
    map_put(m, "ok", js_wb(gcl, 0));
  }
  char *r = cgi_ok(gc, m);
  gc_free(gcl);
  return r;
}

char *cgi_del_user(Gc *gc, char *admin, char *akey, char *user) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  Gc *gcl = gc_new();
  // Map[Js]
  Map *m = map_new(gcl);
  // Opt[char]
  Opt *alevel = check_user(gcl, admin, akey);
  if (opt_is_full(alevel) && str_eq(opt_get(alevel), "0")) {
    del_user(user);
    map_put(m, "ok", js_wb(gcl, 1));
  } else {
    map_put(m, "ok", js_wb(gcl, 0));
  }
  char *r = cgi_ok(gc, m);
  gc_free(gcl);
  return r;
}

char *cgi_change_level(
  Gc *gc, char *admin, char *akey, char *user, char *level
) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  Gc *gcl = gc_new();
  // Map[Js]
  Map *m = map_new(gcl);
  // Opt[char]
  Opt *alevel = check_user(gcl, admin, akey);
  if (opt_is_full(alevel) && str_eq(opt_get(alevel), "0")) {
    if (change_level(user, level)) {
      map_put(m, "ok", js_wb(gcl, 1));
    } else {
      map_put(m, "ok", js_wb(gcl, 0));
    }
  } else {
    map_put(m, "ok", js_wb(gcl, 0));
  }
  char *r = cgi_ok(gc, m);
  gc_free(gcl);
  return r;
}

char *cgi_change_pass(Gc *gc, char *user, char *key, char *new_key) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  Gc *gcl = gc_new();
  // Map[Js]
  Map *m = map_new(gcl);
  if (change_pass(user, key, new_key)) {
    map_put(m, "ok", js_wb(gcl, 1));
  } else {
    map_put(m, "ok", js_wb(gcl, 0));
  }
  char *r = cgi_ok(gc, m);
  gc_free(gcl);
  return r;
}

char *cgi_del_session(Gc *gc, char *session_id) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  del_session(session_id);
  return cgi_empty(gc);
}

char *cgi_authentication(Gc *gc, char *user, char *key, int expiration) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  Gc *gcl = gc_new();

  // Map[Js]
  Map *m = map_new(gcl);

  // Opt[char]
  Opt *level = check_user(gcl, user, key);
  if (opt_is_full(level)) {
    char *session_id = cryp_genk(gcl, klen);
    char *key = cryp_genk(gcl, klen);
    add_session(
      session_id, user, key, expiration ? cgi_null->t_expiration : 0
    );
    map_put(m, "level", js_ws(gcl, opt_get(level)));
    map_put(m, "sessionId", js_ws(gcl, session_id));
    map_put(m, "key", js_ws(gcl, key));
  } else {
    map_put(m, "level", js_ws(gcl, ""));
    map_put(m, "sessionId", js_ws(gcl, ""));
    map_put(m, "key", js_ws(gcl, ""));
  }

  char *r = cgi_ok(gc, m);
  gc_free(gcl);
  return r;
}

char *cgi_connect(Gc *gc, char *session_id) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  Gc *gcl = gc_new();
  // Map[Js]
  Map *m = map_new(gcl);

  set_connection_id(session_id, cryp_genk(gcl, klen));

  char *com_key = "";
  char *con_id = "";
  // Opt[CgiSession]
  Opt *cgiss = read_session(gcl, session_id);
  if (opt_is_full(cgiss)) {
    CgiSession *ss = opt_get(cgiss);
    com_key = ss->com_key;
    con_id = ss->con_id;
  }

  map_put(m, "key", js_ws(gcl, com_key));
  map_put(m, "connectionId", js_ws(gcl, con_id));
  char *r = cgi_ok(gc, m);
  gc_free(gcl);
  return r;
}

// data is Map[Js]
char *cgi_ok(Gc *gc, Map *data) {
  if (!cgi_null)
    EXC_ILLEGAL_STATE("'cgi' has not been intialized")

  Gc *gcl = gc_new();
  char *msg = (char *)js_wo(gcl, data);
  char *key = opt_nget(cgi_null->key);
  if (!key) EXC_ILLEGAL_STATE("Cgi->key is not set")
  char *r = cryp_cryp(gc, msg, key);

/*
  // To debug
  FILE *tmp = fopen("data/tmp.txt", "w");
  fputs(msg, tmp);
  fclose(tmp);


//  fputs(msg, stdout);
*/

  gc_free(gcl);
  return r;
}

char *cgi_empty(Gc *gc) {
  Gc *gcl = gc_new();
  char *r = cgi_ok(gc, map_new(gcl));
  gc_free(gcl);
  return r;
}

char *cgi_error(Gc *gc, char *msg) {
  Gc *gcl = gc_new();
  // Map[Js]
  Map *m = map_new(gcl);
  map_put(m, "error", js_ws(gcl, msg));
  char *r = cgi_ok(gc, m);
  gc_free(gcl);
  return r;
}

char *cgi_expired(Gc *gc) {
  Gc *gcl = gc_new();
  // Map[Js]
  Map *m = map_new(gcl);
  map_put(m, "expired", js_wb(gcl, 1));
  char *r = cgi_ok(gc, m);
  gc_free(gcl);
  return r;
}

// fn_data is Tp[Map[Js] *(*fn)(Map[Js] *rq), GcVal[Map[Js]]]
static void long_run(Tp *fn_ctx) {
  Map *(*lrun)(Map *) = tp_e1(fn_ctx);
  GcVal *ctx = tp_e2(fn_ctx);
  Gc *gc = gcVal_gc(ctx);
  Map *rq = gcVal_value(ctx);
  CGI_GET_STR(gc, long_run_file, rq, "longRunFile")

  // Map[js]
  Map *rp = lrun(rq);
  map_put(rp, "longRunEnd", js_wb(gc, 1));

  // Generate a tmp file to reduce latence time in write 'long_run_file'
  char *tmp = file_tmp(gc, "dmc_cgi_long_run");
  file_write(tmp, (char *)js_wo(gc, rp));
  file_rename(tmp, long_run_file);

  gc_free(gc);
}

// All Maps are Map[Js]. ctx is GcVal[Map[Js]]
char *cgi_long_run(Gc *gc, Map *(*fn)(Map *rq), GcVal *ctx) {
  Gc *gcl = gc_new();
  // Map[Js]
  Map *rq = gcVal_value(ctx);
  CGI_GET_STR(gcl, long_run_file, rq, "longRunFile")

  // Map[Js]
  Map *rp = map_new(gcl);
  if (*long_run_file) {
    if (file_exists(long_run_file)) {
      rp = js_ro(gcl, (Js *)file_read(gcl, long_run_file));
      map_put(rp, "longRunEnd", js_wb(gcl, 1));
      file_del(long_run_file);
    } else {
      map_put(rp, "longRunEnd", js_wb(gcl, 0));
    }
  } else {
    long_run_file = file_tmp(gcl, "dmc_cgi_long_run");

    map_put(rq, "longRunFile", (Js *)js_ws(gcVal_gc(ctx), long_run_file));
    async_run_detached((FPROC)long_run, tp_new(gcVal_gc(ctx), fn, ctx));

    map_put(rp, "longRunFile", (Js *)js_ws(gcl, long_run_file));
  }

  char *r = cgi_ok(gc, rp);
  gc_free(gcl);
  return r;
}
