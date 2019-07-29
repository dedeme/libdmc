// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array of doubles.

#ifndef DMC_DARR_H
  #define DMC_DARR_H

#include "Js.h"

///
typedef struct darr_Darr Darr;

///
Darr *darr_new(void);

/// buffer must be > 0
Darr *darr_bf_new(int buffer);

/// If ix is < 0 then is changed to 'darr_size - ix'
Darr *darr_left(Darr *this, int ix);

/// If ix is < 0 then is changed to 'darr_size - ix'
Darr *darr_right(Darr *this, int ix);

/// If begin or end are < 0 then is changed to 'darr_size - itsValue'
Darr *darr_sub(Darr *this, int begin, int end);

///
Darr *darr_copy(Darr *this);

///
int darr_eq(Darr *this, Darr *other, double gap);

///
int darr_size(Darr *this);

/// If ix is < 0 then is changed to 'darr_size - ix'
double darr_get(Darr *this, int ix);

///
double *darr_start(Darr *this);

///
double *darr_end(Darr *this);

///
void darr_push(Darr *this, double e);

/// If ix is < 0 then is changed to 'darr_size - ix'
void darr_set(Darr *this, int ix, double e);

/// If ix is < 0 then is changed to 'darr_size - ix'
void darr_insert(Darr *this, int ix, double e);

/// If ix is < 0 then is changed to 'darr_size - ix'
void darr_remove(Darr *this, int ix);

///
void darr_cat(Darr *this, Darr *other);

/// If ix is < 0 then is changed to 'darr_size - ix'
void darr_insert_arr(Darr *this, int ix, Darr *other);

/// If begin or end are < 0 then is changed to 'darr_size - itsValue'
void darr_remove_range(Darr *this, int begin, int end);

/// Removes every element of 'this'. Buffer size is equals to 15.
void darr_clear (Darr *this);

/// Removes every element of 'this'.
void darr_bf_clear (Darr *this, int buffer);

///
void darr_reverse(Darr *this);

///
void darr_sort(Darr *this);

///
Js *darr_to_js(Darr *this);

///
Darr *darr_from_js(Js *js);

#endif
