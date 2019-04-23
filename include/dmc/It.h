// Copyright 30-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Iterator.<p>
/// Iterator does not admit NULL values.

#ifndef DM_IT_H
  #define DM_IT_H

#include <stddef.h>

typedef struct opt_Opt Opt;

///
typedef struct it_It It;

///
typedef Opt *(*it_Next)(void *);

/// it_new creates a new It.
///   o: Container
///   next: function which returns a element of 'o' or Opt_empty() if there
///         are no more elements.
It *it_new (void *o, Opt *(*next)(void *o));

///
It *it_empty (void);

///
It *it_unary (void *e);

/// it_range is an iterator that returns values between begin (inclusive)
/// and end (exclusive)
It *it_range (int begin, int end);

/// it_range0 is equals to it_range(0, end);
It *it_range0 (int end);

///
int it_has_next (It *this);

///
void *it_next (It *this);

// Show next element witout advancing.
void *it_peek (It *this);

/// it_add adds an element at the end of 'this'
It *it_add (It *this, void *element);

/// it_add adds an element at the beginning of 'this'
It *it_add0 (It *this, void *element);

///
It *it_cat (It *this, It *another);

///
It *it_take (It *this, size_t n);

///
It *it_takef (It *this, int (*predicate)(void *e));

///
It *it_drop (It *this, size_t n);

///
It *it_dropf (It *this, int (*predicate)(void *e));

///
It *it_filter (It *this, int (*predicate)(void *e));

///
It *it_map (It *this, void *(*converter)(void *e));

/// it_map2 applies conv1 to the first element and conv2 to the others.
It *it_map2 (It *this, void *(*conv1)(void *e), void *(*conv2)(void *e));

/// Returns It[Tp2]
It *it_zip (It *it1, It *it2);

/// Returns It[Tp3]
It *it_zip3 (It *it1, It *it2, It *it3);

///
It *it_reverse (It *this);

/// it_sort sorts 'this' calling 'arr_sort'
It *it_sort (It *this, int (*greater)(void *e1, void *e2));

///
void it_each (It *this, void (*f)(void *e));

///
void it_each_ix (It *this, void (*f)(void *e, size_t ix));

///
size_t it_count (It *this);

///
int it_eq (It *it1, It *it2, int (*feq)(void *e1, void *e2));

///
int it_index (It *this, int (*predicate)(void *e));

///
int it_contains (It *this, int (*predicate)(void *e));

///
int it_last_index (It *this, int (*predicate)(void *e));

/// Returns the first element which satisfies 'predicate' or opt_empty().
Opt *it_find (It *this, int (*predicate)(void *e));

/// Returns the first element which satisfies 'predicate' or 'option'.
void *it_ofind (It *this, void *option, int (*predicate)(void *e));

#endif


