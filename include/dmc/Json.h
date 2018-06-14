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

#include <stdbool.h>
#include <stddef.h>

typedef struct arr_Arr Arr;
typedef struct map_Map Map;
typedef struct ajson_Ajson Ajson;
typedef struct mjson_Mjson Mjson;

///
#define json_err_t "json error"
/// 'Json' is an alias of 'char'
typedef struct json_Json Json;

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
Ajson *json_rarray (Json *json);
///
Mjson *json_robject (Json *json);
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
Json *json_warray(Ajson *a);
///
Json *json_wobject(Mjson *m);

///
void jmap_pnull(Mjson *this, char *key);
///
void jmap_pbool(Mjson *this, char *key, bool value);
///
void jmap_pint(Mjson *this, char *key, int n);
///
void jmap_puint(Mjson *this, char *key, unsigned n);
///
void jmap_pdouble(Mjson *this, char *key, double n, int scale);
/// Admits null values of 's'
void jmap_pstring(Mjson *this, char *key, char *s);
/// Admits null values of 'a'
void jmap_parray(Mjson *this, char *key, Arr *a, Json *(*to)(void *));
/// Admits null values of 'm'
void jmap_pobject(Mjson *this, char *key, Map *m, Json *(*to)(void *));
/// Returns true if key is null. Errors o not existent key make "crashes".
bool jmap_gnull(Mjson *this, char *key);
/// Errors o not existent key make "crashes".
bool jmap_gbool(Mjson *this, char *key);
/// Errors or not existent key make "crashes".
int jmap_gint(Mjson *this, char *key);
/// Errors or not existent key make "crashes".
unsigned jmap_guint(Mjson *this, char *key);
/// Errors or not existent key make "crashes".
double jmap_gdouble(Mjson *this, char *key);
/// Errors or not existent key make "crashes".
char *jmap_gstring(Mjson *this, char *key);
/// Errors or not existent key make "crashes".
Arr *jmap_garray(Mjson *this, char *key, void *(*from)(Json *));
/// Errors or not existent key make "crashes".
Map *jmap_gobject(Mjson *this, char *key, void *(*from)(Json *));

///
void jarr_anull(Ajson *this);
///
void jarr_abool(Ajson *this, bool value);
///
void jarr_aint(Ajson *this, int n);
///
void jarr_auint(Ajson *this, unsigned n);
///
void jarr_adouble(Ajson *this, double n, int scale);
/// Admits null values of 's'
void jarr_astring(Ajson *this, char *s);
/// Admits null values of 'a'
void jarr_aarray(Ajson *this, Arr *a, Json *(*to)(void *));
/// Admits null values of 'm'
void jarr_aobject(Ajson *this, Map *m, Json *(*to)(void *));
/// Returns true if the value 'ix' is null. Errors o not existent key make
/// "crashes".
bool jarr_gnull(Ajson *this, size_t ix);
/// Errors o not existent key make "crashes".
bool jarr_gbool(Ajson *this, size_t ix);
/// Errors or not existent key make "crashes".
int jarr_gint(Ajson *this, size_t ix);
/// Errors or not existent key make "crashes".
unsigned jarr_guint(Ajson *this, size_t ix);
/// Errors or not existent key make "crashes".
double jarr_gdouble(Ajson *this, size_t ix);
/// Errors or not existent key make "crashes".
char *jarr_gstring(Ajson *this, size_t ix);
/// Errors or not existent key make "crashes".
Arr *jarr_garray(Ajson *this, size_t ix, void *(*from)(Json *));
/// Errors or not existent key make "crashes".
Map *jarr_gobject(Ajson *this, size_t ix, void *(*from)(Json *));

///
void jarr_snull(Ajson *this, size_t ix);
///
void jarr_sbool(Ajson *this, size_t ix, bool value);
///
void jarr_sint(Ajson *this, size_t ix, int n);
///
void jarr_suint(Ajson *this, size_t ix, unsigned n);
///
void jarr_sdouble(Ajson *this, size_t ix, double n, int scale);
///
void jarr_sstring(Ajson *this, size_t ix, char *s);
///
void jarr_sarray(Ajson *this, size_t ix, Arr *a, Json *(*to)(void *));
///
void jarr_sobject(Ajson *this, size_t ix, Map *m, Json *(*to)(void *));

#endif

