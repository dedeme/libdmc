// Copyright 05-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "iserver_tests.h"
#include <assert.h>
#include "dmc/async.h"
#include "dmc/Iserver.h"

static void run_server (Iserver *is) {
  for (;;) {
    IserverRq *rq = iserver_read(is);
    if (*iserverRq_error(rq))
      FAIL(iserverRq_error(rq));

    char *msg = ochar_nsome(iserverRq_msg(rq));
    if (msg) {
      assert(str_eq(iserverRq_host(rq), "127.0.0.1"));
      printf("Server-received: %s\nSever-sending: Echo %s\n", msg, msg);
      if (str_eq(msg, "end")) {
        iserverRq_write(rq, str_f("Echo %s & server closed.\n", msg));
        iserver_close(is);
        return;
      }

      /**/void fn (void *rqOk) {
      /**/  sys_sleep(2000);
      /**/  char *msgOk = ochar_some(iserverRq_msg(rqOk));
      /**/  iserverRq_write(rqOk, str_f("Echo %s\n", msgOk));
      /**/}
      async_thread2(fn, rq);
    }
    sys_sleep(1000);
  }
}

static void run_client () {
  for (int n = 0; n < 3; ++n) {
    char *rq = (n < 2) ? str_f("hello %d", n) : "end";
    printf("Cliend-sending: %s\n", rq);
    Ochar *rp = sys_cmd(str_f("printf \"%s\" | nc -q 15 localhost 8888", rq));
    if (ochar_none(rp))
      FAIL(str_f("Fail in sending %d", n));
    printf("Client-received: %s\n", ochar_some(rp));
  }
}

void iserver_tests(int active) {
  puts("Iserver tests:");

  if (active) {
    Iserver *is = iserver_new(8888);

    /**/void fn () { run_server(is); }
    async_thread(fn);
    pthread_t *th = async_thread(run_client);

    async_join(th);
  }

  puts("    Finished");
}
