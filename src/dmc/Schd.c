// Copyright 30-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Schd.h"
#include "dmc/date.h"

struct schd_Task {
  Schd *schd;
  DateTm *deadline;
  void (*fn)(void *);
  void *value;
};

static SchdTask *schd_Task_new (
  Schd *schd, DateTm *deadline, void (*fn)(void *), void *value
) {
  SchdTask *this = MALLOC(SchdTask);
  this->schd = schd;
  this->deadline = deadline;
  this->fn = fn;
  this->value = value;
  return this;
}

struct schd_Ttask {
  Schd *schd;
  SchdTask *task;
  void (*fn)(void *, SchdTask *);
  void *value;
  int interval;
  DateTm *deadline;
};

static struct schd_Ttask *schd_Ttask_new (
  Schd *schd,
  SchdTask *task,
  void (*fn)(void *, SchdTask *),
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

void schdTask_del (SchdTask *this) {
  // ------------------------------------------------------------------------ //
  int ffilter (SchdTask *t) { return t != this; }                             //
  // ------------------------------------------------------------------------ //
  arr_filter((this->schd)->tasks, (FPRED)ffilter);
}

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
  return add(this, schd_Task_new(this, date_tm_now(), fn, value));
}

int schd_exists (Schd *this, SchdTask *task) {
  // ------------------------------------------------------------------------ //
  int ffilter (SchdTask *t) { return t == task; }                             //
  // ------------------------------------------------------------------------ //
  return it_contains(arr_to_it(this->tasks), (FPRED)ffilter);
}

static void finterval (struct schd_Ttask *ttk) {
  SchdTask *tk = ttk->task;
  tk->deadline = date_tm_add(tk->deadline, ttk->interval);
  add(ttk->schd, tk);
  ttk->fn(ttk->value, tk);
}

SchdTask *schd_loop (
  Schd *this,
  void (*fn)(void *value, SchdTask *tk),
  void *value,
  int millis
) {
  if (millis < 0) millis = 0;

  SchdTask *tk = schd_Task_new(this, date_tm_now(), (FPROC)finterval, NULL);

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
    this, schd_Task_new(this, date_tm_add(date_tm_now(), millis), fn, value)
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

// tp is Tp3[
//   Tp3[FILE *popen, void (*fn)(char *), char *fdata],
//   char *fcontrol
//   void *ctx];
static void fcmd (Tp3 *tp, SchdTask *tk) {
  if (file_exists(tp3_e2(tp))) {
    Tp3 *tp3 = (Tp3 *)tp3_e1(tp);
    fclose((FILE *)tp3_e1(tp3));
    void (*fn)(void *, char *) = tp3_e2(tp3);
    fn(tp3_e3(tp), file_read(tp3_e3(tp3)));
    file_del(tp3_e2(tp));
    file_del(tp3_e3(tp3));
    schdTask_del(tk);
  }
}

void schd_cmd (
  Schd *this, void (*fn)(void *ctx, char *result), void *ctx, char *cmd
) {
  char *fdata = file_tmp("dmc_schd");
  char *fcontrol = file_tmp("dmc_schd");
  char *c = str_f("%s > %s 2>&1; touch %s", cmd, fdata, fcontrol);
  FILE *f = popen(c, "r");
  if (!f) {
    fn(ctx, "");
    file_del(fdata);
    file_del(fcontrol);
    return;
  }
  schd_loop(
    this,
    (FLOOP)fcmd,
    tp3_new(tp3_new(f, fn, fdata), fcontrol, ctx),
    50
  );
}
