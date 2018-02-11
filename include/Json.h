// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing json strings.<p>
/// Functions which start with 'json_' are direct converters from (json_rXXX) or
/// to (json_wXXX) Json.<br>
/// Functions which start with 'jmap_' are shortcuts to save data in Maps.
/// 'jmap_p' saves a value and 'jmap_g' returns it.<br>
/// Functions which start with 'jarr_' are shortcuts to serialization of
/// objects. 'jit_' are shortcuts to restore objects serialized with 'jarr_'.<p>
/// Functions 'jmap_', 'jarr_' and 'jit_' do not allow null values. These must
/// be implemented with generic 'json_' functions.

#ifndef DM_COM_JSON_H
# define DM_COM_JSON_H

#include "Map.h"

///
typedef char Json;

/// Returns true if json is "null" or false in another case.
bool json_rnull (Json *json);
///
bool json_rbool (Json *json);
///
int json_rint (Json *json);
///
unsigned json_ruint (Json *json);
///
double json_rdouble (Json *json);
/// .
char *json_rstring (Json *json);
///
Arr/*Json*/ *json_rarray (Json *json);
///
Map/*Json*/ *json_robject (Json *json);
///
Json *json_wnull(void);
///
Json *json_wbool(bool value);
///
Json *json_wint(int n);
///
Json *json_wuint(unsigned n);
/// Scale is forced between [0 - 9] inclusive
Json *json_wdouble(double n, int scale);
///
Json *json_wstring(char *s);
///
Json *json_warray(Arr *a);
///
Json *json_wobject(Map *m);

///
void jmap_pbool(Map/*Json*/ *this, char *key, bool value);
///
void jmap_pint(Map/*Json*/ *this, char *key, int n);
///
void jmap_puint(Map/*Json*/ *this, char *key, unsigned n);
///
void jmap_pdouble(Map/*Json*/ *this, char *key, double n, int scale);
/// Admits null values of 's'
void jmap_pstring(Map/*Json*/ *this, char *key, char *s);
/// Admits null values of 'a'
void jmap_parray(Map/*Json*/ *this, char *key, Arr/*Json*/ *a);
/// Admits null values of 'm'
void jmap_pobject(Map/*Json*/ *this, char *key, Map/*Json*/ *m);
/// Errors o not existent key make "crashes".
bool jmap_gbool(Map/*Json*/ *this, char *key);
/// Errors or not existent key make "crashes".
int jmap_gint(Map/*Json*/ *this, char *key);
/// Errors or not existent key make "crashes".
unsigned jmap_guint(Map/*Json*/ *this, char *key);
/// Errors or not existent key make "crashes".
double jmap_gdouble(Map/*Json*/ *this, char *key);
/// Errors or not existent key make "crashes".
char *jmap_gstring(Map/*Json*/ *this, char *key);
/// Errors or not existent key make "crashes".
Arr/*Json*/ *jmap_garray(Map/*Json*/ *this, char *key);
/// Errors or not existent key make "crashes".
Map/*Json*/ *jmap_gobject(Map/*Json*/ *this, char *key);

///
void jarr_bool(Arr/*Json*/ *this, bool value);
///
void jarr_int(Arr/*Json*/ *this, int n);
///
void jarr_uint(Arr/*Json*/ *this, unsigned n);
///
void jarr_double(Arr/*Json*/ *this, double n, int scale);
/// Admits null values of 's'
void jarr_string(Arr/*Json*/ *this, char *s);
/// Admits null values of 'a'
void jarr_array(Arr/*Json*/ *this, Arr/*Json*/ *a);
/// Admits null values of 'm'
void jarr_object(Arr/*Json*/ *this, Map/*Json*/ *m);
/// Errors o not existent key make "crashes".
bool jit_bool(It/*Json*/ *this);
/// Errors or not existent key make "crashes".
int jit_int(It/*Json*/ *this);
/// Errors or not existent key make "crashes".
unsigned jit_uint(It/*Json*/ *this);
/// Errors or not existent key make "crashes".
double jit_double(It/*Json*/ *this);
/// Errors or not existent key make "crashes".
char *jit_string(It/*Json*/ *this);
/// Errors or not existent key make "crashes".
Arr/*Json*/ *jit_array(It/*Json*/ *this);
/// Errors or not existent key make "crashes".
Map/*Json*/ *jit_object(It/*Json*/ *this);

#endif

