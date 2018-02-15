// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Iterator.<br>
/// Utilities for managing iterators.<br>
/// List:<br>
/// <table>
/// <tr><td colspan="4"><hr></td></tr>
/// <tr><td colspan="4"><b>Constructors</b></td></tr>
/// <tr>
/// <td width="200" valign="top">
/// <a href="#hp:it_from">it_from</a><br>
/// <a href="#hp:it_empty">it_empty</a><br>
/// </td><td width="200" valign="top">
/// <a href="#hp:it_new">it_new</a><br>
/// <a href="#hp:it_range">it_range</a><br>
/// </td>
/// </td><td width="200" valign="top">
/// <a href="#hp:it_range0">it_range0</a><br>
/// <a href="#hp:it_unary">it_unary</a><br>
/// </td>
/// </tr>
/// <tr><td colspan="4"><hr></td></tr>
/// <tr><td colspan="4"><b>Object</b></td></tr>
/// <tr>
/// </td><td valign="top">
/// <a href="#hp:it_has_next">it_has_next</a><br>
/// </td><td valign="top">
/// <a href="#hp:it_next">it_next</a><br>
/// </td><td valign="top">
/// <a href="#hp:it_peek">it_peek</a><br>
/// </td>
/// </tr>
/// <tr><td colspan="4"><hr></td></tr>
/// <tr><td colspan="4"><b>Converters</b></td></tr>
/// <tr>
/// <td valign="top">
/// <a href="#hp:it_cat">it_cat</a><br>
/// <a href="#hp:it_drop">it_drop</a><br>
/// <a href="#hp:it_dropf">it_dropf</a><br>
/// <a href="#hp:it_filter">it_filter</a><br>
/// </td><td valign="top">
/// <a href="#hp:it_map">it_map</a><br>
/// <a href="#hp:it_reverse">it_reverse <sup>*</su></a><br>
/// <a href="#hp:it_sort_locale">it_rsort_locale <sup>*</su></a><br>
/// <a href="#hp:it_sort_str">it_rsort_str <sup>*</su></a><br>
/// </td><td valign="top">
/// <a href="#hp:it_sort">it_sort <sup>*</su></a><br>
/// <a href="#hp:it_sort_locale">it_sort_locale <sup>*</su></a><br>
/// <a href="#hp:it_sort_str">it_sort_str <sup>*</su></a><br>
/// </td><td valign="top">
/// <a href="#hp:it_take">it_take</a><br>
/// <a href="#hp:it_takef">it_takef</a><br>
/// <a href="#hp:it_to_peek">it_to_peek</a><br>
/// </td>
/// </tr>
/// <tr><td colspan="4"><hr></td></tr>
/// <tr><td colspan="4"><b>Consumers</b></td></tr>
/// <tr>
/// <td valign="top">
/// <a href="#hp:it_find">it_find</a><br>
/// <a href="#hp:it_contains">it_contains</a><br>
/// <a href="#hp:it_contains_str">it_contains_str</a><br>
/// <a href="#hp:it_count">it_count</a><br>
/// </td><td valign="top">
/// <a href="#hp:it_each">it_each</a><br>
/// <a href="#hp:it_each_ix">it_each_ix</a><br>
/// <a href="#hp:it_eq">it_eq</a><br>
/// </td><td valign="top">
/// <a href="#hp:it_eq_str">it_eq_str</a><br>
/// <a href="#hp:it_index">it_index</a><br>
/// <a href="#hp:it_index">it_index_str</a><br>
/// </td><td valign="top">
/// <a href="#hp:it_last_index">it_last_index</a><br>
/// <a href="#hp:it_last_index">it_last_index_str</a><br>
/// <a href="#hp:it_to">it_to</a><br>
/// </td>
/// </tr>
/// <tr><td colspan="4"><hr></td></tr>
/// </table>
/// <p><sup>*</sup> Note: <i>These converters are not lazy, since all of them
/// construct an intermediate array.</i></p>

#ifndef DM_IT_H
  #define DM_IT_H

#include <stddef.h>
#include <stdbool.h>

///
#define it_from arr_to_it

///
#define it_to arr_from_it

///
typedef struct it_It It;

/// Iterator that allows seeing its next element without advancing
typedef It ItPeek;

///
It *it_new(
  void *o,
  bool (*has_next)(void *o),
  void *(*next)(void *o)
);

///
It *it_empty(void);

///
It *it_unary(void *e);

/// Iterator that returns values between begin (inclusive) and end (exclusive)
It *it_range(int begin, int end);

/// Equals to it_range(0, end);
It *it_range0(int end);

/// Converts this in a ItPeek
ItPeek *it_to_peek(It *this);

///
bool it_has_next (It *this);

///
void *it_next (It *this);

/// Show next element witout advancing.<br>
/// Only must be used with Iterators type ItPeek. This kind of iterators is
/// returned by <tt>it_to_peek()</tt> and <tt>it_dropf()</tt>
void *it_peek (ItPeek *this);

///
It *it_cat (It *this, It *another);

///
It *it_take (It *this, size_t n);

///
It *it_takef (It *this, bool (*predicate)(void *e));

///
It *it_drop (It *this, size_t n);

///
ItPeek *it_dropf (It *this, bool (*predicate)(void *e));

///
It *it_filter (It *this, bool (*predicate)(void *e));

///
It *it_map (It *this, void *(*converter)(void *e));

/// it_map2 applies conv1 to the first element and conv2 to the others.
It *it_map2 (It *this, void *(*conv1)(void *e), void *(*conv2)(void *e));

///
It *it_zip (It *it1, It *it2);

///
It *it_zip3 (It *it1, It *it2, It *it3);

///
It *it_reverse (It *this);

///
It *it_sort (It *this, int (*comparator)(void *e1, void *e2));

///
It *it_sort_str (It *this);

///
It *it_rsort_str (It *this);

///
It *it_sort_locale (It *this);

///
It *it_rsort_locale (It *this);

///
void it_each (It *this, void (*f)(void *e));

///
void it_each_ix (It *this, void (*f)(void *e, size_t ix));

///
size_t it_count (It *this);

///
bool it_eq (It *it1, It *it2, bool (*feq)(void *e1, void *e2));

///
bool it_eq_str (It *it1, It *it2);

///
int it_index (It *this, bool (*predicate)(void *e));

///
bool it_contains (It *this, bool (*predicate)(void *e));

///
int it_last_index (It *this, bool (*predicate)(void *e));

///
int it_index_str (It *this, char *s);

///
bool it_contains_str (It *this, char *s);

///
int it_last_index_str (It *this, char *s);

/// Returns firs element which satisfies 'predicate' or NULL.
void *it_find (It *this, bool (*predicate)(void *e));

/// Serializes this
char *it_serialize (It *this, char *(*serialize)(void *));

/// Restores this
It *it_restore (char *serial, void *(*restore)(char *));

#endif


