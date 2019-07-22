// Copyright 22-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for HTML conections between client - server

#ifndef DMC_CGI_H
  #define DMC_CGI_H

#include "dmc/std.h"

/// Return of cgi_get_session_data
typedef struct cgi_Session CgiSession;

/// Returns comunication key
char *cgiSession_key (CgiSession *this);

/// Returns connection identifier
char *cgiSession_id (CgiSession *this);

/// cgi_klen returns the standard length of keys.
int cgi_klen(void);

/// cgi_init initializes a new interface of commnications.
///   gc          : Garbage collector
///   home        : Aboslute path of application directory. For example:
///                   "/peter/wwwcgi/dmcgi/JsMon"
///                   or
///                   "/home/deme/.dmCApp/JsMon" )
///   t_expiration: Time in seconds.
void cgi_init(Gc *gc, char *home, time_t t_expiration);

///
char *cgi_home(void);

/// cgi_set_key sets the key which 'cgi_ok' and 'cgi_err' will use. This
/// function is called when connection or authentication.
void cgi_set_key(char *k);

/// cgi_get_key returns Opt[CgiSession].<br>
/// If 'session_id' is not valid, it returns 'opt_empty'.
Opt *cgi_get_session(Gc *gc, char *session_id);

/// cgi_add_usser adds an user to database and retuns a response.
///   admin: Admin name
///   akey: Admin password
///   user: New user name
///   ukey: New user password
///   level: New user level. Level "0" is the admin one.
///   return: A boolean field {ok:true|false}, sets to true if
///     operation succeeded. A fail can come up if 'admin' authentication fails.
char *cgi_add_user(
  Gc *gc,
  char *admin, char *akey,
  char *user, char *ukey, char *level
);

/// cgi_del_user remove an user from database and returns a response
///   admin: Admin name
///   akey: Admin password
///   user: User name to remove
///   return: A boolean field {ok:true|false}, sets to true if
///     operation succeeded. A fail can come up if 'admin' authentication fails.
char *cgi_del_user(Gc *gc, char *admin, char *akey, char *user);

/// cgi_change_level modify a user level and returns a response
///   admin: Admin name
///   akey: Admin password
///   user: User name to change level
///   level: New user level. Level "0" is the admin one.
///   return: A boolean field {ok:true|false}, sets to true if
///     operation succeeded. A fail can come up if 'admin' authentication fails
///     or 'user' does not exist.
char *cgi_change_level(
  Gc *gc, char *admin, char *akey, char *user, char *level
);

/// cgi_change pass change a user password and returns a response
///   user: User name to change password
///   key: Old password
///   new_key: New password
///   return: A boolean field {ok:true|false}, sets to true if
///     operation succeeded. A fail can come up if 'user' authentication fails.
char *cgi_change_pass(Gc *gc, char *user, char *key, char *new_key);

/// cgi_del_session deletes 'session' and returns an empty response.
char *cgi_del_session(Gc *gc, char *session_id);

/// cgi_authentication send to client level, key, page_id and session_id.
/// If authentication failed every value is "".
///   user: User name
///   key: User password
///   expiration: If is set to false, session will expire after 30 days.
///   return: 'level', 'key', 'pageId' and 'sessionId' fields.
char *cgi_authentication(Gc *gc, char *user, char *key, int expiration);

/// cig_connect returns client 'connectionId' and 'key'. If conection failed
/// both are "".
///   session_id: Session identifier
///   return: {connectionId: String, key: String}
///           'key' is a new key, set for the new connection.
char *cgi_connect(Gc *gc, char  *session_id);

/// cgi_ok returns a normal response.<br>
/// 'data' is a Map[Js]
char *cgi_ok(Gc *gc, Map *data);

/// cgi_empty retuns an empty response.
char *cgi_empty(Gc *gc);

/// cgi_ok returns an error response, setting {error:msg}.
char *cgi_error(Gc *gc, char *msg);

/// cgi_expired returns a expired response, setting {expired:1}
char *cgi_expired(Gc *gc);

/// Runs a "long run" task. This function is intended to be called until it
/// returns {"longRunEnd"='true'}.
///   Gc: Garbage collector to add the 'char *' returned
///   fn: char *(*)(Map[Js] *rq). "Long run" task. <i>It must
///       not be defined as inner function</i>.
///   rq: [Map[Js]. Value extracted of GcVal.
///   ctx: GcVal[Map[Js]]. Map[Js] have data for 'fn'. It must have a field
///       called "longRunFile" which value the first time it is called is ""
///       and after that its value is the returned by this function.
///       'ctx' will be freed automtically when "fn' finalizes.
///   return: a cgi_ok with
///     first call     : An only field "longRunFile" whitch must
///                      be used in following calls.
///     following calls: - If 'fn' was finished, the reponse returned with 'fn'
///                        with the field {"longRunEnd"='true'} added.
///                      - If 'fn' is running a response with the only field
///                        {"longRunEnd"='false'}
char *cgi_long_run(Gc *gc, Map *(*fn)(Map *rq), GcVal *ctx);

#endif
