// Copyright 05-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/cgi.h"
#include "dmc/rnd.h"
#include "dmc/cryp.h"
#include "dmc/date.h"
#include "dmc/async.h"

// Constants -------------------------------------------------------------------

// Keys length
static size_t cgi_klen_ = 300;
// Time when user selects a session with not expiration.
static time_t cgi_t_no_expiration = 2592000; // seconds == 30 days
// Initial key ("deme")
static char *cgi_deme_key =
    "nkXliX8lg2kTuQSS/OoLXCk8eS4Fwmc+N7l6TTNgzM1vdKewO0cjok51vcdl"
    "OKVXyPu83xYhX6mDeDyzapxL3dIZuzwyemVw+uCNCZ01WDw82oninzp88Hef"
    "bn3pPnSMqEaP2bOdX+8yEe6sGkc3IO3e38+CqSOyDBxHCqfrZT2Sqn6SHWhR"
    "KqpJp4K96QqtVjmXwhVcST9l+u1XUPL6K9HQfEEGMGcToMGUrzNQxCzlg2g+"
    "Hg55i7iiKbA0ogENhEIFjMG+wmFDNzgjvDnNYOaPTQ7l4C8aaPsEfl3sugiw";
// Key when there is no session
static char *cgi_no_session_key = "nosession";

// Variables -------------------------------------------------------------------

// Key to write files.
static char *cgi_fkey = NULL;
// Session key
static Ochar *cgi_key = NULL;
// Home directory
static char *cgi_home_ = NULL;
// Time (in seconds) to session expiration
static time_t cgi_t_expiration = 0;

// Data ------------------------------------------------------------------------

struct cgi_User {
  char *pass;
  char *level;
};

typedef struct cgi_User CgiUser;

CgiUser *cgi_user_new(char *pass, char *level) {
  CgiUser *this = MALLOC(CgiUser);
  this->pass = pass;
  this->level = level;
  return this;
}

char *cgi_user_to_js(CgiUser *this) {
  return js_wa(achar_new_from(
    js_ws(this->pass),
    js_ws(this->level),
    NULL
  ));
}

CgiUser *cgi_user_from_js(char *js) {
  char **es = js_ra(js)->es;
  return cgi_user_new(
    js_rs(es[0]),
    js_rs(es[1])
  );
}

struct cgi_Session {
  char *key; // Communication key
  char *con_key; // Connectin key
  char *user; // User id
  char *level; // User level
  time_t time; // Last communication time
  int lapse; // Time of expiration
};

/// Data of a session.
typedef struct cgi_Session CgiSession;

CgiSession *cgi_session_new (
  char *key, char *con_key, char *user, char *level, time_t time, int lapse
) {
  CgiSession *this = MALLOC(CgiSession);
  this->key = key;
  this->con_key = con_key;
  this->user = user;
  this->level = level;
  this->time = time;
  this->lapse = lapse;
  return this;
}

char *cgi_session_to_js(CgiSession *this) {
  return js_wa(achar_new_from(
    js_ws(this->key),
    js_ws(this->con_key),
    js_ws(this->user),
    js_ws(this->level),
    js_wl(this->time),
    js_wi(this->lapse),
    NULL
  ));
}

CgiSession *cgi_session_from_js(char *js) {
  char **es = js_ra(js)->es;
  return cgi_session_new(
    js_rs(es[0]),
    js_rs(es[1]),
    js_rs(es[2]),
    js_rs(es[3]),
    js_rl(es[4]),
    js_ri(es[5])
  );
}

/// If 's' is out of date, returns 0, otherwise updates session time and
/// returns 1.
int cgi_session_update (CgiSession *s) {
  time_t now = date_now();
  time_t new = s->time + s->lapse;

  if (now > new) {
    return 0;
  }
  s->time = new;
  return 1;
}

// Db --------------------------------------------------------------------------

void cgi_users_write(Mchar *users) {
  file_write(
    path_cat(cgi_home_, "users.tb", NULL),
    cryp_cryp(cgi_fkey, js_wo(users))
  );
}

Mchar *cgi_users_read (void) {
  return js_ro(cryp_decryp(cgi_fkey, file_read(
    path_cat(cgi_home_, "users.tb", NULL
  ))));
}

void cgi_sessions_write(Mchar *sessions) {
  file_write(
    path_cat(cgi_home_, "sessions.tb", NULL),
    cryp_cryp(cgi_fkey, js_wo(sessions))
  );
}

Mchar *cgi_sessions_read (void) {
  return js_ro(cryp_decryp(cgi_fkey, file_read(
    path_cat(cgi_home_, "sessions.tb", NULL
  ))));
}

void cgi_sessions_purge (Mchar *ss_js) {
  /**/int obsolete (Kchar *s_js) {
  /**/  CgiSession *s = cgi_session_from_js(s_js->v);
  /**/  return date_now() <= s->time + s->lapse;
  /**/}
  aKchar_filter_in(mchar_to_array(ss_js), obsolete);
}

// Public interface ------------------------------------------------------------

void cgi_init(char *home, time_t t_expiration) {
  rnd_init();

  cgi_fkey = cryp_key(cgi_deme_key, strlen(cgi_deme_key));
  cgi_key = ochar_mk_none();
  cgi_home_ = home;
  cgi_t_expiration = t_expiration;

  if (!file_exists(home)) {
    file_mkdir(home);
  }

  char *fusers = path_cat(home, "users.tb", NULL);
  if (!file_exists(fusers)) {
    Mchar *m = mchar_new();
    cgi_sessions_write(m);
    mchar_put(m, "admin", cgi_user_to_js(cgi_user_new(
      cryp_key(cgi_deme_key, cgi_klen_), "0")
    ));
    cgi_users_write(m);
  }

}

char *cgi_home () {
  return cgi_home_;
}

int cgi_klen () {
  return cgi_klen_;
}

char *cgi_authentication(
  char *key, char *user, char *pass, int with_expiration
) {
  cgi_key = ochar_mk_some(key);

  Mchar *rp = mchar_new();
  mchar_put(rp, "sessionId", js_ws(""));
  mchar_put(rp, "key", js_ws(""));
  mchar_put(rp, "conKey", js_ws(""));
  mchar_put(rp, "level", js_ws(""));

  char *u_js = ochar_nsome(mchar_get(cgi_users_read(), user));
  if (u_js) {
    CgiUser *u = cgi_user_from_js(u_js);
    if (str_eq(u->pass, cryp_key(pass, cgi_klen_))) {
      char *session_id = cryp_genk(cgi_klen_);
      char *comKey = cryp_genk(cgi_klen_);
      char *conKey = cryp_genk(cgi_klen_);

      Mchar *ss_js = cgi_sessions_read();
      cgi_sessions_purge(ss_js);
      mchar_put(ss_js, session_id, cgi_session_to_js(cgi_session_new(
        comKey, conKey, user, u->level, date_now(),
        with_expiration ? cgi_t_expiration : cgi_t_no_expiration
      )));
      cgi_sessions_write(ss_js);

      mchar_put(rp, "sessionId", js_ws(session_id));
      mchar_put(rp, "key", js_ws(comKey));
      mchar_put(rp, "conKey", js_ws(conKey));
      mchar_put(rp, "level", js_ws(u->level));
    }
  }
  return cgi_rp(rp);
}

char *cgi_connect(char *session_id) {
  cgi_key = ochar_mk_some(session_id);

  Mchar *rp = mchar_new();
  mchar_put(rp, "key", js_ws(""));
  mchar_put(rp, "conKey", js_ws(""));
  mchar_put(rp, "user", js_ws(""));
  mchar_put(rp, "level", js_ws(""));

  Mchar *ss_js = cgi_sessions_read();
  char *s_js = ochar_nsome(mchar_get(ss_js, session_id));
  if (s_js) {
    CgiSession *s = cgi_session_from_js(s_js);
    if (cgi_session_update(s)) {
      mchar_put(ss_js, session_id, cgi_session_to_js(s));
      cgi_sessions_write(ss_js);

      mchar_put(rp, "key", js_ws(s->key));
      mchar_put(rp, "conKey", js_ws(s->con_key));
      mchar_put(rp, "user", js_ws(s->user));
      mchar_put(rp, "level", js_ws(s->level));
    }
  }
  return cgi_rp(rp);
}

Ochar *cgi_set_key (char *sessionId, char *con_key) {
  Mchar *ss_js = cgi_sessions_read();
  char *s_js = ochar_nsome(mchar_get(ss_js, sessionId));
  if (s_js) {
    CgiSession *s = cgi_session_from_js(s_js);
    if (cgi_session_update(s) && (!*con_key || str_eq(con_key, s->con_key))) {
      mchar_put(ss_js, sessionId, cgi_session_to_js(s));
      cgi_sessions_write(ss_js);
      cgi_key = ochar_mk_some(s->key);
      return cgi_key;
    }
  }
  return ochar_mk_none();
}

char *cgi_change_pass (char *user, char *old, char *new) {
  Mchar *rp = mchar_new();
  mchar_put(rp, "ok", js_wb(0));

  Mchar *us_js = cgi_users_read();
  char *u_js = ochar_nsome(mchar_get(us_js, user));
  if (u_js) {
    CgiUser *u = cgi_user_from_js(u_js);
    if (str_eq(u->pass, cryp_key(old, cgi_klen_))) {
      u->pass = cryp_key(new, cgi_klen_);
      mchar_put(us_js, user, cgi_user_to_js(u));
      cgi_users_write(us_js);

      mchar_put(rp, "ok", js_wb(1));
    }
  }

  return cgi_rp(rp);
}

int cgi_add_user (char *user, char *pass, char *level) {
  Mchar *us_js = cgi_users_read();
  if (ochar_none(mchar_get(us_js, user))) {
    mchar_put(us_js, user, cgi_user_to_js(
      cgi_user_new(cryp_key(pass, cgi_klen_), level)
    ));
    cgi_users_write(us_js);
    return 1;
  }
  return 0;
}

void cgi_remove_user (char *user) {
  Mchar *us_js = cgi_users_read();
  mchar_remove(us_js, user);
  cgi_users_write(us_js);
}

void cgi_remove_session (char *sessionId) {
  Mchar *ss_js = cgi_sessions_read();
  mchar_remove(ss_js, sessionId);
  cgi_sessions_write(ss_js);
}

char *cgi_rp (Mchar *rp) {
  return cryp_cryp(ochar_some(cgi_key), js_wo(rp));
}

char *cgi_rp_empty (void) {
  return cryp_cryp(ochar_some(cgi_key), js_wo(mchar_new()));
}

char *cgi_rp_error (char *msg) {
  Mchar *rp = mchar_new();
  mchar_put(rp, "error", js_ws(msg));
  return cryp_cryp(ochar_some(cgi_key), js_wo(rp));
}

char *cgi_rp_expired (void) {
  Mchar *rp = mchar_new();
  mchar_put(rp, "expired", js_wb(1));
  return cryp_cryp(cgi_no_session_key, js_wo(rp));
}

int cgi_rq_bool (Mchar *rq, char *key) {
  return js_rb(ochar_some(mchar_get(rq, key)));
}

int cgi_rq_int (Mchar *rq, char *key) {
  return js_ri(ochar_some(mchar_get(rq, key)));
}

long cgi_rq_long (Mchar *rq, char *key) {
  return js_rl(ochar_some(mchar_get(rq, key)));
}

double cgi_rq_double (Mchar *rq, char *key) {
  return js_rd(ochar_some(mchar_get(rq, key)));
}

char *cgi_rq_string (Mchar *rq, char *key) {
  return js_rs(ochar_some(mchar_get(rq, key)));
}

Achar *cgi_rq_array (Mchar *rq, char *key) {
  return js_ra(ochar_some(mchar_get(rq, key)));
}

Mchar *cgi_rq_object (Mchar *rq, char *key) {
  return js_ro(ochar_some(mchar_get(rq, key)));
}
