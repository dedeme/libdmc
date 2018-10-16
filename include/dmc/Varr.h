// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DMC_VARR_H
  #define DMC_VARR_H

///
typedef struct varr_Varr Varr;

///
Varr *varr_new(void);

/// buffer must be > 0
Varr *varr_2_new(int buffer);

/// If ix is < 0 then is changed to 'varr_size - ix'
Varr *varr_left_new(Varr *this, int ix);

/// If ix is < 0 then is changed to 'varr_size - ix'
Varr *varr_right_new(Varr *this, int ix);

/// If begin or end are < 0 then is changed to 'varr_size - itsValue'
Varr *varr_sub_new(Varr *this, int begin, int end);

///
void varr_free(Varr *this);

///
int varr_size(Varr *this);

/// If ix is < 0 then is changed to 'varr_size - ix'
void *varr_get(Varr *this, int ix);

///
void **varr_start(Varr *this);

///
void **varr_end(Varr *this);

///
void varr_push(Varr *this, void *e);

/// If ix is < 0 then is changed to 'varr_size - ix'
void varr_set(Varr *this, int ix, void *e);

/// If ix is < 0 then is changed to 'varr_size - ix'
void varr_insert(Varr *this, int ix, void *e);

/// If ix is < 0 then is changed to 'varr_size - ix'
void varr_remove(Varr *this, int ix);

///
void varr_cat(Varr *this, Varr *other);

/// If ix is < 0 then is changed to 'varr_size - ix'
void varr_insert_arr(Varr *this, int ix, Varr *other);

/// If begin or end are < 0 then is changed to 'varr_size - itsValue'
void varr_remove_range(Varr *this, int begin, int end);

///
void varr_reverse(Varr *this);

///
void varr_sort(Varr *this, int (*greater)(void *, void *));

#endif
