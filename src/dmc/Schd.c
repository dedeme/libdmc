// Copyright 30-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Schd.h"
#include "dmc/date.h"

struct schd_Task {
  DateTm *deadline;
  void (*fn)(void *);
  void *value;
};

static SchdTask *schd_Task_new (
  DateTm *deadline, void (*fn)(void *), void *value
) {
  SchdTask *this = MALLOC(SchdTask);
  this->deadline = deadline;
  this->fn = fn;
  this->value = value;
  return this;
}

struct schd_Ttask {
  Schd *schd;
  SchdTask *task;
  void (*fn)(void *);
  void *value;
  int interval;
  DateTm *deadline;
};

static struct schd_Ttask *schd_Ttask_new (
  Schd *schd,
  SchdTask *task,
  void (*fn)(void *),
  void *value,
  int interval
) {
  struct schd_Ttask *this = MALLOC(struct schd_Ttask);
  this->schd = schd;
  this->task = task;
  this->fn = fn;
  this->value = value;
  this->interval = interval;
  return this;
}

struct schd_Schd {
  // Arr [SchdTask]
  Arr *tasks;
  int active;
};

Schd *schd_new (void) {
  Schd *this = MALLOC(Schd);
  this->tasks = arr_new();
  this->active = 1;
  return this;
}

static SchdTask *add (Schd *this, SchdTask *tk) {
  DateTm *tm = tk->deadline;
  int ix = 0;
  // Arr[SchdTask]
  Arr *tasks = this->tasks;
  EACH(tasks, SchdTask, t)
    if (date_tm_df(t->deadline, tm) > 0) {
      break;
    }
    ++ix;
  _EACH
  arr_insert(tasks, ix, tk);
  return tk;
}

SchdTask *schd_add (Schd *this, void (*fn)(void *), void *value) {
  return add(this, schd_Task_new(date_tm_now(), fn, value));
}

void schd_del (Schd *this, SchdTask *task) {
  // ------------------------------------------------------------------------ //
  int ffilter (SchdTask *t) { return t != task; }                             //
  // ------------------------------------------------------------------------ //
  arr_filter(this->tasks, (FPRED)ffilter);
}

int schd_exists (Schd *this, SchdTask *task) {
  // ------------------------------------------------------------------------ //
  int ffilter (SchdTask *t) { return t == task; }                             //
  // ------------------------------------------------------------------------ //
  return it_contains(arr_to_it(this->tasks), (FPRED)ffilter);
}

static void finterval (struct schd_Ttask *ttk) {
  ttk->fn(ttk->value);
  SchdTask *tk = ttk->task;
  tk->deadline = date_tm_add(tk->deadline, ttk->interval);
  add(ttk->schd, tk);
}

SchdTask *schd_interval (
  Schd *this, void (*fn)(void *), void *value, int millis
) {
  if (millis < 4) millis = 4;

  SchdTask *tk = schd_Task_new(date_tm_now(), (FPROC)finterval, NULL);

  struct schd_Ttask *ttask = schd_Ttask_new(
    this, tk, fn, value, millis
  );

  tk->value = ttask;
  return add(this, tk);
}

SchdTask *schd_delay (
  Schd *this, void (*fn)(void *), void *value, int millis
) {
  return add(
    this, schd_Task_new(date_tm_add(date_tm_now(), millis), fn, value)
  );
}

void schd_start (Schd *this) {
  // Arr [SchdTask]
  Arr *tks = this->tasks;
  while (this->active && arr_size(tks)) {
    SchdTask *t = *arr_start(tks);
    arr_remove(tks, 0);
    int df = date_tm_df(t->deadline, date_tm_now());
    if (df > 0) {
      GC_gcollect();
    }
    df = date_tm_df(t->deadline, date_tm_now());
    if (df > 0) {
      sys_sleep(df);
    }
    t->fn(t->value);
  }
}

void schd_end (Schd *this) {
  this->active = 0;
}

