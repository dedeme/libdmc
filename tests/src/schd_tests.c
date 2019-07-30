// Copyright 30-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "schd_tests.h"
#include <assert.h>
#include "dmc/Schd.h"
#include "dmc/date.h"
#include "dmc/rnd.h"
#include "dmc/ext.h"

static void barbery (void) {

  // Constants -----------------------------------

  int TIME_MAX_CLIENT_CREATE = 2000;
  int TIME_MIN_CLIENT_CREATE = 200;
  int TIME_HAIR_CUT = 1800;
  int TIME_OPEN = 120000;

  // Schd ----------------------------------------

  Schd *sc = schd_new();

  // Barbery and its -----------------------------

  int is_closed = 1;

  int corder = 1;
  int SIT_C = 5;
  int client_sits[SIT_C];
  int order_sits[SIT_C];
  int *cp = client_sits;
  int *op = order_sits;
  REPEAT(SIT_C)
    *cp++ = 0;
    *op++ = 0;
  _REPEAT

  int barbery_is_empty (void) {
    int *cp = client_sits;
    REPEAT(SIT_C)
      if (*cp++) return 0;
    _REPEAT
    return 1;
  }

  int get_a_sit (int *client) {
    RANGE0(i, SIT_C)
      if (!client_sits[i]) {
        client_sits[i] = *client;
        order_sits[i] = corder++;
        return 1;
      };
    _RANGE
    return 0;
  }

  // Opt[int]
  Opt *get_client () {
    int sel = -1;
    int sel_order = 0;
    RANGE0(i, SIT_C)
      if (client_sits[i]) {
        if (sel < 0 || order_sits[i] < sel_order) {
          sel = i;
          sel_order = order_sits[i];
        }
      }
    _RANGE
    if (sel < 0) {
      return opt_empty();
    }

    int *r = ATOMIC(sizeof(int));
    *r = client_sits[sel];

    client_sits[sel] = 0;

    return opt_new(r);
  }

  char *sits_to_str (void) {
    // Arr[char]
    Arr *cs = arr_new();
    RANGE0(i, SIT_C)
      arr_push(cs, str_f("%d", client_sits[i]));
    _RANGE
    return str_f("[%s]", str_join(cs, ", "));
  }

  // Clock ---------------------------------------

  int is_close_time = 0;

  void fclock (void *null_value) {
    is_close_time = 1;
  }

  schd_delay(sc, fclock, NULL, TIME_OPEN);

  // Barber --------------------------------------

  int is_occupy = 0;

  void bmsg(char *msg) {
    printf("Barber: %s\n", msg);
  }

  void barber_start (void *null_value) {
    bmsg("Openning barbery");
    is_closed = 0;
    is_occupy = 0;
  }

  void hair_cut_end (Tp *client_fstart) {
    bmsg(str_f("End cutting hair to %d", *((int *)tp_e1(client_fstart))));
    bmsg("Watching for clients");
    // Opt[int]
    Opt *client = get_client();
    if (opt_is_empty(client)) {
      if (is_close_time) {
        is_closed = 1;
        bmsg("No clientes and time out: Barbery closed");
      } else {
        is_occupy = 0;
        bmsg("No clients: sleeping");
      }
      return;
    }
    if (is_close_time && !is_closed) {
      bmsg("Close barbery");
      is_closed = 1;
    }
    bmsg(str_f("Taking the client %d", *((int *)opt_get(client))));
    puts(sits_to_str());
    is_occupy = 1;
    schd_add(sc, tp_e2(client_fstart), opt_get(client));
  }

  void hair_cut_start (int *client) {
    is_occupy = 1;
    bmsg(str_f("Start cutting hair to %d. ✄✄✄✄", *client));
    schd_delay(
      sc, (FPROC)hair_cut_end, tp_new(client, hair_cut_start), TIME_HAIR_CUT
    );
  }

  schd_add(sc, barber_start, NULL);

  // Clients -------------------------------------

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
        schd_add(sc, (FPROC)hair_cut_start, client);
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

  DateTm *time_to_new_client = date_tm_now();

  void fclients (void *null_pointer) {
    if (is_close_time) {
      return;
    }

    int *client = mk_client();
    client_run(client);

    time_to_new_client = date_tm_add(time_to_new_client,
      TIME_MIN_CLIENT_CREATE +
      rnd_i(TIME_MAX_CLIENT_CREATE - TIME_MIN_CLIENT_CREATE)
    );
    int df = date_tm_df(time_to_new_client, date_tm_now());
    schd_delay(sc, fclients, NULL, df);
  }

  schd_add(sc, fclients, NULL);

  // Start Schd ----------------------------------

  schd_start(sc);

}

void schd_tests(void) {
  puts("Schd tests");


  Schd *sc = schd_new();

  puts ("before");

  int *vinterval = ATOMIC(sizeof(int));
  *vinterval = 0;
  void finterval (int *c, SchdTask *tk) {
    printf("%d\n", *c);
    *c += 1;
    if (*c == 20) {
      schdTask_del(tk);
    }
  }
  schd_loop(sc, (FLOOP)finterval, vinterval, 500);

  void fcmd (void *null, char *page) {
    puts(page);
  }
  ext_awget(sc, fcmd, NULL, "http://www.invertia.com");

  schd_start(sc);


  if (0) barbery();

  puts( "    Finished");
}
