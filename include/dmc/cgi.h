// Copyright 05-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for HTML conections between client - server.

#ifndef DMC_CGI_H
  #define DMC_CGI_H

#include <time.h>
#include "dmc/char/Mchar.h"

/// Initializes a new interface of commnications.
///   home        : Path of application directory. For example:
///                 "dmcgi/JsMon"
///                 or
///                 "/home/deme/.dmCApp/JsMon" ).
///   t_expiration: Time in seconds.
void cgi_init (char *home, time_t t_expiration);

/// Returns the path of the application directory.
char *cgi_home (void);

/// Returns standard length of passwords.
int cgi_klen ();

/// Sends to client 'sessionId', 'communicationKey' and 'userLevel'. If
/// conection fails every one is "".
///   key            : Communication key
///   user           : User id.
///   pass           : User password.
///   with_expiration: If is set to 0, session will expire after 30 days.
///   RETURN         : {sessionId: String, key: String, conKey: String,
///                     level: String}.
char *cgi_authentication (
  char *key, char *user, char *pass, int with_expiration
);

/// Sends to client session data:
/// If conection fails every field is "".
///    session_id: Session identifier.
///    RETURN    : {key: String, conKey: String, user: String, level: String}
char *cgi_connect (char *session_id);

/// Sets the session communication and returns it. If fails it returns none.
///   sessionId: Session identifier.
///   con_key  : Connection key. If its value is "", this parameter is not used.
Ochar *cgi_set_key (char *sessionId, char *con_key);

/// Changes 'user' password. If the change fails returns a response {ok=false}.
///   user  : User identifier.
///   old   : Old password.
///   new   : New password.
///   RETURN: {ok: Boolean}
char *cgi_change_pass (char *user, char *old, char *new);

/// Adds a new user. If the user identifier already exists, it returns 0.
/// Otherwise it returns 1.
///   user : User id.
///   pass : User password.
///   level: User level.
int cgi_add_user (char *user, char *pass, char *level);

/// Remove user if exists.
///   user : User id.
void cgi_remove_user (char *user);

/// Remove sessin if exists.
///   sessionId: Session identifier.
void cgi_remove_session (char *sessionId);

/// Returns a response to send to client.
///   rp: Response.
char *cgi_rp (Mchar *rp);

/// Returns an empty response.
char *cgi_rp_empty (void);

/// Returns a message with an only field "error" with value 'msg'.
///   msg: Error message.
char *cgi_rp_error (char *msg);

/// Returns a message with an only field "expired" with value 'true',
/// codified with the key 'noSessionKey' ("nosession")
char *cgi_rp_expired (void);

/// Read a JSON Boolean from a JSON object. It raises a fail if it is not
/// possible to read the field.
int cgi_rq_bool (Mchar *rq, char *key);

/// Read a JSON Int from a JSON object. It raises a fail if it is not
/// possible to read the field.
int cgi_rq_int (Mchar *rq, char *key);

/// Read a JSON Long from a JSON object. It raises a fail if it is not
/// possible to read the field.
long cgi_rq_long (Mchar *rq, char *key);

/// Read a JSON Double from a JSON object. It raises a fail if it is not
/// possible to read the field.
double cgi_rq_double (Mchar *rq, char *key);

/// Read a JSON String from a JSON object. It raises a fail if it is not
/// possible to read the field.
char *cgi_rq_string (Mchar *rq, char *key);

/// Read a JSON Array[JSON] from a JSON object. It raises a fail if it is not
/// possible to read the field.
Achar *cgi_rq_array (Mchar *rq, char *key);

/// Read a JSON Object[JSON] from a JSON object. It raises a fail if it is not
/// possible to read the field.
Mchar *cgi_rq_object (Mchar *rq, char *key);

#endif
