// Copyright 01-May-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "iserver_tests.h"
#include <assert.h>
#include "dmc/async.h"
#include "dmc/Iserver.h"

static void run_server (Iserver *is) {
  for (;;) {
    IserverRq *rq = iserver_read(is);
    if (*iserverRq_error(rq))
      EXC_IO(iserverRq_error(rq))

    if (opt_is_full(iserverRq_msg(rq))) {
      assert(str_eq(iserverRq_host(rq), "127.0.0.1"));
      char *msg = opt_get(iserverRq_msg(rq));
      printf("Server-received: %s\nSever-sending: Echo %s\n", msg, msg);
      if (str_eq(msg, "end")) {
        iserverRq_write(rq, str_f("Echo %s & server closed.\n", msg));
        iserver_close(is);
        return;
      }

      iserverRq_write(rq, str_f("Echo %s\n", msg));
    }
    sys_sleep(200);
  }
}

static void run_client (void *null) {
  RANGE0(n, 3)
    char *rq = (n < 2) ? str_f("hello %d", n) : "end";
    printf("Cliend-sending: %s\n", rq);
    // Opt[char]
    Opt *rp = sys_cmd(str_f("printf \"%s\" | nc -q 15 localhost 8888", rq));
    if (opt_is_empty(rp))
      EXC_IO(str_f("Fail in sending %d", n))
    printf("Client-received: %s\n", (char *)opt_get(rp));
  _RANGE
}

void iserver_tests(int active) {
  puts("Iserver tests:");

  if (active) {
    Iserver *is = iserver_new(8888);

    async_thread((FPROC)run_server, is);
    pthread_t *th = async_thread(run_client, NULL);

    async_join(th);
  }

  puts("    Finished");
}
