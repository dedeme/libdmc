// Copyright 15-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef DMC_ARR_H
  #define DMC_ARR_H

///
typedef struct arr_Arr Arr;

///
Arr *arr_new(void(*ffree)(void *));

/// buffer must be > 0
Arr *arr_2_new(int buffer, void(*ffree)(void *));

/// If ix is < 0 then is changed to 'arr_size - ix'
Arr *arr_left_new(Arr *this, int ix, void *(*copy_new)(void *));

/// If ix is < 0 then is changed to 'arr_size - ix'
Arr *arr_right_new(Arr *this, int ix, void *(*copy_new)(void *));

/// If begin or end are < 0 then is changed to 'arr_size - itsValue'
Arr *arr_sub_new(Arr *this, int begin, int end, void *(*copy_new)(void *));

///
void arr_free(Arr *this);

///
int arr_size(Arr *this);

/// If ix is < 0 then is changed to 'arr_size - ix'
void *arr_get(Arr *this, int ix);

///
void **arr_start(Arr *this);

///
void **arr_end(Arr *this);

///
void arr_push(Arr *this, void *e);

/// If ix is < 0 then is changed to 'arr_size - ix'
void arr_set(Arr *this, int ix, void *e);

/// If ix is < 0 then is changed to 'arr_size - ix'
void arr_insert(Arr *this, int ix, void *e);

/// If ix is < 0 then is changed to 'arr_size - ix'
void arr_remove(Arr *this, int ix);

///
void arr_cat(Arr *this, Arr *other, void *(*copy_new)(void *));

/// If ix is < 0 then is changed to 'arr_size - ix'
void arr_insert_arr(Arr *this, int ix, Arr *other, void *(*copy_new)(void *));

/// If begin or end are < 0 then is changed to 'arr_size - itsValue'
void arr_remove_range(Arr *this, int begin, int end);

///
void arr_reverse(Arr *this);

///
void arr_sort(Arr *this, int (*greater)(void *, void *));

#endif
