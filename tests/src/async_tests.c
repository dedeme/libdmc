// Copyright 29-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "async_tests.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "dmc/async.h"
#include "dmc/date.h"
#include "dmc/rnd.h"
#include "dmc/OInt.h"
#include "dmc/char/Achar.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "dmc/err.h"
#include "dmc/sys.h"

static void barbery (void) {
  int TIME_MAX_CLIENT_CREATE = 2000; // millis
  int TIME_MIN_CLIENT_CREATE = 200; // millis
  int TIME_HAIR_CUT = 1800; // millis
  int TIME_OPEN = 120; // seconds

  int is_closed = 1;
  int is_occupy = 0;

  AsyncActor *actor = asyncActor_new(50);

  int corder = 1;
  int SIT_C = 5;
  int client_sits[SIT_C];
  int order_sits[SIT_C];
  int *cp = client_sits;
  int *op = order_sits;
  for (int i = 0; i < SIT_C; ++i) {
    *cp++ = 0;
    *op++ = 0;
  }

  int barbery_is_empty (void) {
    int *cp = client_sits;
    for (int i = 0; i < SIT_C; ++i) if (*cp++) return 0;
    return 1;
  }

  int get_a_sit (int *client) {
    for (int i = 0; i < SIT_C; ++i) {
      if (!client_sits[i]) {
        client_sits[i] = *client;
        order_sits[i] = corder++;
        return 1;
      };
    }
    return 0;
  }

  OInt *get_client () {
    int sel = -1;
    int sel_order = 0;
    for (int i = 0; i < SIT_C; ++i) {
      if (client_sits[i]) {
        if (sel < 0 || order_sits[i] < sel_order) {
          sel = i;
          sel_order = order_sits[i];
        }
      }
    }
    if (sel < 0) {
      return oInt_mk_none();
    }

    int r = client_sits[sel];
    client_sits[sel] = 0;

    return oInt_mk_some(r);
  }

  char *sits_to_str (void) {
    Achar *cs = achar_new();
    for (int i = 0; i < SIT_C; ++i)
      achar_push(cs, str_f("%d", client_sits[i]));
    return str_f("[%s]", str_join(cs, ", "));
  }

  time_t clock = 0;

  void clock_init (void) {
    clock = date_now();
  }

  int is_time_out (void) {
    if (!clock)
      FAIL("Clock is 0");
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
    OInt *client = get_client();
    if (oInt_none(client)) {
      if (is_time_out()) {
        is_closed = 1;
        asyncActor_end(actor);
        bmsg("No clientes and time out: Barbery closed");
      } else {
        is_occupy = 0;
        bmsg("No clients: sleeping");
      }
      return;
    }
    bmsg("Taking a client");
    puts(sits_to_str());
    is_occupy = 1;
    int *pclient = ATOMIC(sizeof(int *));
    *pclient = oInt_some(client);
    async_thread_detached((void(*)(void*))hair_cut, pclient);
  }

  void hair_cut (int *client) {
    bmsg(str_f("Start cutting hair to %d", *client));
    sys_sleep(TIME_HAIR_CUT);
    bmsg(str_f("End cutting hair to %d", *client));
    asyncActor_run(actor, (void(*)(void *))watch_sites, hair_cut);
  }

  void barber_end (void) {
    is_closed = 1;
  }

  int clientc = 1;

  void cmsg(int *client, char *msg) {
    printf("Client %d: %s\n", *client, msg);
  }

  int *mk_client () {
    int *this = ATOMIC(sizeof(int));
    *this = clientc++;
    cmsg(this, "Created");
    return this;
  }

  void client_run (int *client) {
    if (is_closed) {
      cmsg(client, "Go away because barbery is closed");
      return;
    }

    if (barbery_is_empty()) {
      if (is_occupy) {
        get_a_sit(client);
        cmsg(client, "Take a sit");
        puts(sits_to_str());
      } else {
        cmsg(client, "Calling to barber");
        is_occupy = 1;
        async_thread_detached((void(*)(void *))hair_cut, client);
      }
    } else {
      if (get_a_sit(client)) {
        cmsg(client, "Take a sit");
        puts(sits_to_str());
      } else {
        cmsg(client, "Go away because there is no sit");
      }
    }
  }

  clock_init();
  barber_start();

  int delay = 0;

  void create_clients (void *null_value) {
    if (delay == 0) {
      asyncActor_run(actor, (void(*)(void *))client_run, mk_client());
      delay = (
          TIME_MIN_CLIENT_CREATE +
          rnd_i(TIME_MAX_CLIENT_CREATE - TIME_MIN_CLIENT_CREATE)
        ) / 50;
      return;
    }
    --delay;
  }

  AsyncTimer *timer_clients = asyncTimer_new(create_clients, NULL, 50);

  AsyncTimer *timer_clock = NULL;

  void timer_clock_end (void *null_value) {
    if (is_time_out()) {
      puts("Time out");
      asyncTimer_end(timer_clients);
      barber_end();
      asyncTimer_end(timer_clock);
    }
  }

  void clock_control (void *null_value) {
    asyncActor_run(actor, timer_clock_end, NULL);
  }

  timer_clock = asyncTimer_new(clock_control, NULL, 50);

  asyncActor_join(actor);
}

static void counter (char *c) {
  int n = atoi(c);
  for (int i = 0; i < n; ++i) {
    printf("%d\n", i);
    sys_sleep(50);
  }
}

void async_tests(void) {
  puts("Async tests");

/*
  void fn() { puts("Hello"); }
  pthread_t *thr = async_thread(fn);
  async_join(thr);
*/

  if (0) {
    async_thread_detached((void(*)(void *))counter, "10");
    async_thread_detached((void(*)(void *))counter, "10");
    sys_sleep(1000);
  }
/*
  AsyncActor *ac = asyncActor_new(50);
  void task (void *null) {
    puts("--------");
    for (int i = 0; i < 10; ++i) {
      printf("%d\n", i);
      sys_sleep(10);
    }
  }
  void end (void *null) {
    asyncActor_end(ac);
  }
  asyncActor_run(ac, task, NULL);
  puts("Main thread 1");

  void fn2 () { task(NULL); }
  asyncActor_wait(ac, fn2);
  puts("Main thread 2");

  asyncActor_run(ac, end, NULL);

  asyncActor_join(ac);
*/
/*
  void fn2(void *null) { sys_sleep(10000);}
  for (int i = 0; i < 1000000; ++i) async_thread_detached(fn2, NULL);

  puts ("All thread run");
  char c = getchar();
  printf("%c|\n", c);
*/
  if (0) barbery();

  puts("    Finished");
}
