// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utilities for managing json strings.<p>
/// Functions which start with 'json_' are direct converters from (json_rXXX) or
/// to (json_wXXX) Json.<br>
/// Functions which start with 'jmap_' are shortcuts to save data in Maps.
/// 'jmap_p' saves a value and 'jmap_g' returns it.<br>
/// Functions which start with 'jarr_' are shortcuts to save data in Arr's.
/// 'jarr_a' adds a new value, 'jarr_p' saves a value in a position and
/// 'jmap_g' returns it.<br>
/// Functions 'jmap_gXXX' and 'jarr_gXXX' do not return null values.
/// These must be checked with 'jmap_gnull' or 'jarr_gnull'.

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
Json *json_warray(Arr/*Json*/ *a);
///
Json *json_wobject(Map/*Json*/ *m);

///
void jmap_pnull(Map/*Json*/ *this, char *key);
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
void jmap_parray(Map/*Json*/ *this, char *key, Arr *a, Json *(*to)(void *));
/// Admits null values of 'm'
void jmap_pobject(Map/*Json*/ *this, char *key, Map *m, Json *(*to)(void *));
/// Returns true if key is null. Errors o not existent key make "crashes".
bool jmap_gnull(Map/*Json*/ *this, char *key);
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
Arr *jmap_garray(Map/*Json*/ *this, char *key, void *(*from)(Json *));
/// Errors or not existent key make "crashes".
Map *jmap_gobject(Map/*Json*/ *this, char *key, void *(*from)(Json *));

///
void jarr_anull(Arr/*Json*/ *this);
///
void jarr_abool(Arr/*Json*/ *this, bool value);
///
void jarr_aint(Arr/*Json*/ *this, int n);
///
void jarr_auint(Arr/*Json*/ *this, unsigned n);
///
void jarr_adouble(Arr/*Json*/ *this, double n, int scale);
/// Admits null values of 's'
void jarr_astring(Arr/*Json*/ *this, char *s);
/// Admits null values of 'a'
void jarr_aarray(Arr/*Json*/ *this, Arr *a, Json *(*to)(void *));
/// Admits null values of 'm'
void jarr_aobject(Arr/*Json*/ *this, Map *m, Json *(*to)(void *));
/// Returns true if the value 'ix' is null. Errors o not existent key make
/// "crashes".
bool jarr_gnull(Arr/*Json*/ *this, size_t ix);
/// Errors o not existent key make "crashes".
bool jarr_gbool(Arr/*Json*/ *this, size_t ix);
/// Errors or not existent key make "crashes".
int jarr_gint(Arr/*Json*/ *this, size_t ix);
/// Errors or not existent key make "crashes".
unsigned jarr_guint(Arr/*Json*/ *this, size_t ix);
/// Errors or not existent key make "crashes".
double jarr_gdouble(Arr/*Json*/ *this, size_t ix);
/// Errors or not existent key make "crashes".
char *jarr_gstring(Arr/*Json*/ *this, size_t ix);
/// Errors or not existent key make "crashes".
Arr *jarr_garray(Arr/*Json*/ *this, size_t ix, void *(*from)(Json *));
/// Errors or not existent key make "crashes".
Map *jarr_gobject(Arr/*Json*/ *this, size_t ix, void *(*from)(Json *));

///
void jarr_snull(Arr/*Json*/ *this, size_t ix);
///
void jarr_sbool(Arr/*Json*/ *this, size_t ix, bool value);
///
void jarr_sint(Arr/*Json*/ *this, size_t ix, int n);
///
void jarr_suint(Arr/*Json*/ *this, size_t ix, unsigned n);
///
void jarr_sdouble(Arr/*Json*/ *this, size_t ix, double n, int scale);
///
void jarr_sstring(Arr/*Json*/ *this, size_t ix, char *s);
///
void jarr_sarray(Arr/*Json*/ *this, size_t ix, Arr *a, Json *(*to)(void *));
///
void jarr_sobject(Arr/*Json*/ *this, size_t ix, Map *m, Json *(*to)(void *));

#endif

