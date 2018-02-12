// Copyright 12-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for HTML conections between client - server

#ifndef DM_CGI_H
  #define DM_CGI_H

#include <time.h>
#include <Json.h>

/// cgi_klen returns the standard length of keys.
int cgi_klen();

/// CgiRp is a Map[Json] serialized with 'json_wobject()' and encrypted with
/// 'cryp_cryp()' using the key of 'cgi_set_key()'.<br>
/// The Map[Json] has ever a field called 'error' set to "" in ok responses
/// and to a message if there is some error.
typedef char CgiRp;

///
typedef struct cgi_Cgi Cgi;

/// cgi_new creates a new interface of commnications.
///   home        : Aboslute path of application directory below wwwcgi/dmcgi
///                 directory. (e.g. /peter/wwwcgi/dmcgi/JsMon)
///   t_expiration: Time in seconds.
Cgi *cgi_new(char *home, time_t t_expiration);

///
char *cgi_home(Cgi *this);

/// cgi_set_key sets the key which 'cgi_ok' and 'cgi_err' will use. This
/// function is called when connection or authentication.
void cgi_set_key(Cgi* this, char *k);

/// cgi_get_page_id_key returns page_id and key. If session_id is wrong,
/// both are "". This function is calle when normal requests.
void cgi_get_page_id_key(
  char **page_id, char **key, Cgi *this, char *session_id
);

/// cgi_add_usser adds an user to database
///   this:
///   admin: Admin name
///   akey: Admin password
///   user: New user name
///   ukey: New user password
///   level: New user level. Level "0" is the admin one.
///   return: Response with a boolean field {ok:true|false}, sets to true if
///     operation succeeded. A fail can come up if 'admin' authentication fails.
CgiRp *cgi_add_user(
  Cgi *this, char *admin, char *akey, char *user, char *ukey, char *level
);

/// cgi_del_user remove an user from database
///   this:
///   admin: Admin name
///   akey: Admin password
///   user: User name to remove
///   return: Response with a boolean field {ok:true|false}, sets to true if
///     operation succeeded. A fail can come up if 'admin' authentication fails.
CgiRp *cgi_del_user(Cgi *this, char *admin, char *akey, char *user);

/// cgi_change_level modify a user level
///   this:
///   admin: Admin name
///   akey: Admin password
///   user: User name to change level
///   level: New user level. Level "0" is the admin one.
///   return: Response with a boolean field {ok:true|false}, sets to true if
///     operation succeeded. A fail can come up if 'admin' authentication fails
///     or 'user' does not exist.
CgiRp *cgi_change_level(
  Cgi *this, char *admin, char *akey, char *user, char *level
);

/// cgi_change pass change a user password
///   this:
///   user: User name to change password
///   key: Old password
///   new_key: New password
///   return: Response with a boolean field {ok:true|false}, sets to true if
///     operation succeeded. A fail can come up if 'user' authentication fails.
CgiRp *cgi_change_pass(Cgi *this, char *user, char *key, char *new_key);

/// cgi_del_session deletes 'session' and return a no relevant response.
CgiRp *cgi_del_session(Cgi *this, char *session_id);

/// cgi_authentication send client level, key, page_id and session_id.
/// If authentication failed every value is "".
///   this:
///   user: User name
///   key: User password
///   expiration: If is set to false, session will expire after 30 days.
///   return: Response with 'level', 'key', 'pageId' and 'sessionId' fields.
CgiRp *cgi_authentication(Cgi *this, char *user, char *key, bool expiration);

/// cig_connect send client 'pageId' and 'key'. If conection failed both are
/// "".<br>
/// 'key' is a new key, set for the new connection.
CgiRp *cgi_connect(Cgi *this, char  *session_id);

/// cgi_ok creates a new CgiRp, setting {error:""}.
CgiRp *cgi_ok(Cgi *this, Map/*Json*/ *data);

/// cgi_ok creates a new CgiRp, setting {error:msg}.
CgiRp *cgi_error(Cgi *this, char *msg);

/// cgi_expired creates a new CgiRp, setting {expired:true, error:""}
CgiRp *cgi_expired(Cgi *this);

#endif
