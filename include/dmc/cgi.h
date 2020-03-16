// Copyright 18-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for HTML conections between client - server.

#ifndef DMC_CGI_H
  #define DMC_CGI_H

#include "dmc/std.h"

/// Returns of cgi_get_session_data.
typedef struct cgi_Session CgiSession;

/// Returns sesion user.
char *cgiSession_user(CgiSession *this);

/// Returns sesion user level.
char *cgiSession_level(CgiSession *this);

/// Returns comunication key.
char *cgiSession_key(CgiSession *this);

/// Returns the standard length of keys.
int cgi_klen(void);

/// Initializes a new interface of commnications.
///   home        : Aboslute path of application directory. For example:
///                   "/peter/wwwcgi/dmcgi/JsMon"
///                   or
///                   "/home/deme/.dmCApp/JsMon" ).
///   t_expiration: Time in seconds.
void cgi_init(char *home, time_t t_expiration);

/// Returns 'home' directory.
char *cgi_home(void);

/// Sets the key which 'cgi_ok' and 'cgi_err' will use. This function is called
/// when connection or authentication.
void cgi_set_key(char *k);

/// Returns Opt<CgiSession>.
/// If 'session_id' is not valid, it returns 'opt_empty'.
Opt *cgi_get_session(char *session_id);

/// Adds an user to database.
///   admin : Admin name.
///   akey  : Admin password.
///   user  : New user name.
///   ukey  : New user password.
///   level : New user level. Level "0" is the admin one.
///   return: A boolean field {ok:true|false}, sets to true if operations
///           succeeded. A fail can come up if 'admin' authentication fails.
char *cgi_add_user(
  char *admin, char *akey,
  char *user, char *ukey, char *level
);

/// Removea an user from database.
///   admin : Admin name.
///   akey  : Admin password.
///   user  : User name to remove.
///   return: A boolean field {ok:true|false}, sets to true if
///           operation succeeded. A fail can come up if 'admin' authentication
///           fails.
char *cgi_del_user(char *admin, char *akey, char *user);

/// Modifies user level.
///   admin : Admin name.
///   akey  : Admin password.
///   user  : User name to change level.
///   level : New user level. Level "0" is the admin one.
///   return: A boolean field {ok:true|false}, sets to true if
///           operation succeeded. A fail can come up if 'admin' authentication
///           fails or 'user' does not exist.
char *cgi_change_level(
  char *admin, char *akey, char *user, char *level
);

/// Changes user password.
///   user   : User name to change password.
///   key    : Old password.
///   new_key: New password.
///   return : A boolean field {ok:true|false}, sets to true if operation
///            succeeded. A fail can come up if 'user' authentication fails.
char *cgi_change_pass(char *user, char *key, char *new_key);

/// cgi_del_session deletes 'session' and returns an empty response.
char *cgi_del_session(char *session_id);

/// Sends to client level, key, page_id and session_id.
/// If authentication failed every value is "".
///   user      : User name.
///   key       : User password.
///   expiration: If is set to false, session will expire after 30 days.
///   return    : 'level', 'key', 'pageId' and 'sessionId' fields.
char *cgi_authentication(char *user, char *key, int expiration);

/// Returns client 'connectionId' and 'key'. If conection failed both are "".
///   session_id: Session identifier.
///   return    : {connectionId: String, key: String}.
///               'key' is a new key, set for the new connection.
char *cgi_connect(char  *session_id);

/// Returns a normal response.
/// 'data' is a Map[Js]
char *cgi_ok(Map *data);

/// Retuns an empty response.
char *cgi_empty(void);

/// Returns an error response, setting {error:msg}.
char *cgi_error(char *msg);

/// Returns a expired response, setting {expired:1}.
char *cgi_expired(void);

/// Runs a "long run" task. This function is intended to be called until it
/// returns {"longRunEnd"='true'}.
///   fn    : Map[Js] *(*)(void *ctx, Map[Js *rq]). "Long run" task. <i>It must
///           not be defined as inner function</i>.
///   ctx   : Context. It can be NULL. Value to pass to fn.
///   rq    : Map[js]. Data for 'fn'. 'rq' must have a field called
///           "longRunFile" which value the first time it is called is "" and
///           after
///           that its value is the returned by this function.
///   return:
///     first call     : A Map[Js] with an only field "longRunFile" whitch must
///                      be used in following calls.
///     following calls: - If 'fn' was finished the Map returned with 'fn' with
///                        the field {"longRunEnd"='true'} added.
///                      - If 'fn' is running a Map with the only field
///                        {"longRunEnd"='false'}
Map *cgi_long_run(Map *(*fn)(void *ctx, Map *rq), void *ctx, Map *rq);
#endif
