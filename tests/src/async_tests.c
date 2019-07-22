// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "async_tests.h"
#include <assert.h>
#include "dmc/async.h"
#include "dmc/date.h"
#include "dmc/rnd.h"

struct Client {
  Gc *gc;
  int n;
};
static struct Client *client_new (int n) {
  Gc *gc = gc_new();
  struct Client *this = gc_add(gc, malloc(sizeof(struct Client)));
  this->gc = gc;
  this->n = n;
  return this;
}

static void barbery (void) {
  Gc *gc = gc_new();

  int TIME_MAX_CLIENT_CREATE = 2000; // millis
  int TIME_MIN_CLIENT_CREATE = 200; // millis
  int TIME_HAIR_CUT = 1800; // millis
  int TIME_OPEN = 15; // seconds

  int is_closed = 1;
  int is_occupy = 0;

  AsyncActor *actor = asyncActor_new(gc, 50);

  int corder = 1;
  int SIT_C = 5;
  // Opt [struct Client]
  Opt *client_sits[SIT_C];
  int order_sits[SIT_C];
  // Opt [struct Client]
  Opt **cp = client_sits;
  int *op = order_sits;
  REPEAT(SIT_C)
    *cp++ = opt_empty();
    *op++ = 0;
  _REPEAT

  int barbery_is_empty (void) {
    Opt **cp = client_sits;
    REPEAT(SIT_C)
      if (opt_is_empty(*cp++)) return 1;
    _REPEAT
    return 0;
  }

  int get_a_sit (struct Client *client) {
    RANGE0(i, SIT_C)
      if (opt_is_empty(client_sits[i])) {
        client_sits[i] = opt_new(client);
        order_sits[i] = corder++;
        return 1;
      };
    _RANGE
    return 0;
  }

  // Opt[Client]
  Opt *get_client (void) {
    int sel = -1;
    int sel_order = 0;
    RANGE0(i, SIT_C)
      if (opt_is_full(client_sits[i])) {
        if (sel < 0 || order_sits[i] < sel_order) {
          sel = i;
          sel_order = order_sits[i];
        }
      }
    _RANGE
    if (sel < 0) {
      return opt_empty();
    }

    Opt *r = client_sits[sel];

    client_sits[sel] = opt_empty();

    return r;
  }

  char *sits_to_str (Gc *gc) {
    Gc *gcl = gc_new();
    // Arr[char]
    Arr *cs = arr_new(gcl);
    RANGE0(i, SIT_C)
      struct Client *cl = opt_nget(client_sits[i]);
      arr_push(cs, str_f(gcl, "%d", cl ? cl->n : 0));
    _RANGE

    char *r = str_f(gc, "[%s]", str_join(gcl, cs, ", "));
    gc_free(gcl);
    return r;
  }

  time_t clock = 0;

  void clock_init (void) {
    clock = date_now();
  }

  int is_time_out (void) {
    if (!clock)
      EXC_ILLEGAL_STATE("Clock is 0")

    return date_now() - clock > TIME_OPEN;
  }

  void bmsg(char *msg) {
    printf("Barber: %s\n", msg);
  }

  void barber_start (void) {
    bmsg("Openning barbery");
    is_closed = 0;
  }

  void watch_sites(void *(hair_cut)(int *c)) {
    bmsg("Watching for clients");
    struct Client *client = opt_nget(get_client());
    if (is_time_out()){
      is_closed = 1;
      if (!client) {
        asyncActor_end(actor);
        bmsg("No clientes and time out: Barbery closed");
        return;
      }
    }
    if (!client) {
      is_occupy = 0;
      bmsg("No clients: sleeping");
      return;
    }
    Gc *gc = gc_new();
    bmsg("Taking a client");
    puts(sits_to_str(gc));
    is_occupy = 1;
    async_run_detached((FPROC)hair_cut, client);
    gc_free(gc);
  }

  void hair_cut (struct Client *client) {
    Gc *gc = gc_new();
    bmsg(str_f(gc, "Start cutting hair to %d", client->n));
    sys_sleep(TIME_HAIR_CUT);
    bmsg(str_f(gc, "End cutting hair to %d", client->n));
    gc_free(client->gc);
    gc_free(gc);
    asyncActor_run(actor, (FPROC)watch_sites, hair_cut);
  }

  void barber_end (void) {
    is_closed = 1;
  }

  int clientc = 1;

  void cmsg(struct Client *client, char *msg) {
    printf("Client %d: %s\n", client->n, msg);
  }

  struct Client *mk_client () {
    struct Client *this = client_new(clientc++);
    cmsg(this, "Created");
    return this;
  }

  void client_run (struct Client *client) {
    if (is_closed) {
      cmsg(client, "Go away because barbery is closed");
      gc_free(client->gc);
      return;
    }

    Gc *gc = gc_new();
    if (barbery_is_empty()) {
      if (is_occupy) {
        get_a_sit(client);
        cmsg(client, "Take a sit");
        puts(sits_to_str(gc));
      } else {
        cmsg(client, "Calling to barber");
        is_occupy = 1;
        async_run_detached((FPROC)hair_cut, client);
      }
    } else {
      if (get_a_sit(client)) {
        cmsg(client, "Take a sit");
        puts(sits_to_str(gc));
      } else {
        cmsg(client, "Go away because there is no sit");
        gc_free(client->gc);
      }
    }
    gc_free(gc);
  }

  clock_init();
  barber_start();

  int delay = 0;

  void create_clients (void *null_value) {
    if (delay == 0) {
      asyncActor_run(actor, (FPROC)client_run, mk_client());
      delay = (
          TIME_MIN_CLIENT_CREATE +
          rnd_i(TIME_MAX_CLIENT_CREATE - TIME_MIN_CLIENT_CREATE)
        ) / 50;
      return;
    }
    --delay;
  }

  AsyncTimer *timer_clients = asyncTimer_new(gc, 50);
  asyncTimer_run(timer_clients, create_clients, NULL);

  AsyncTimer *timer_clock = asyncTimer_new(gc, 50);

  void end_clients (void *null) {
    sys_sleep(3000);
    asyncTimer_end(timer_clients);
  }

  void timer_clock_end (void *null_value) {
    if (is_time_out()) {
      async_run_detached(end_clients, NULL);
      barber_end();
      puts("Barbery closed");
      asyncTimer_end(timer_clock);
    }
  }

  void clock_control (void *null_value) {
    asyncActor_run(actor, timer_clock_end, NULL);
  }

  asyncTimer_run(timer_clock, clock_control, NULL);

  asyncActor_join(actor);

  gc_free(gc);
}

void async_tests(void) {
  puts("Async tests");
  Gc *gc = gc_new();

/*
  void fn(char *tx) { puts(tx); }
  pthread_t *thr = async_new(gc);
  async_run(thr, (FPROC)fn, "Hello");
  async_join(thr);

  AsyncActor *ac = asyncActor_new(gc, 50);
  void task (void *null) {
    puts("--------");
    RANGE0(i, 10)
      printf("%d\n", i);
      sys_sleep(10);
    _RANGE
  }
  void end (void *th) {
    asyncActor_end(ac);
  }
  asyncActor_run(ac, task, NULL);
  puts("Main thread 1");

  asyncActor_wait(ac, task, NULL);
  puts("Main thread 2");

  asyncActor_run(ac, end, NULL);

  asyncActor_join(ac);
*/

  if (0) barbery();

  gc_free(gc);
  puts("    Finished");
}

