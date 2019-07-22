// Copyright 22-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Iserver.h"
#include "dmc/std.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>

struct iserver_Rq {
  int sock;
  // Opt[char]
  Opt *msg;
  // Opt[char]
  Opt *host;
};

// msg and host are Opt[char]
static IserverRq *iserverRq_new (Gc *gc, int sock, Opt *msg, Opt *host) {
  IserverRq *this = gc_add(gc, malloc(sizeof(IserverRq)));
  this->sock = sock;
  this->msg = msg;
  this->host = host;
  return this;
}

char *iserverRq_error (IserverRq *this) {
  return this->sock < 0 ? opt_get(this->msg) : "";
}

Opt *iserverRq_msg (IserverRq *this) {
  if (this->sock < 0)
    EXC_ILLEGAL_STATE("No request has been read")
  return this->msg;
}

char *iserverRq_host (IserverRq *this) {
  if (opt_is_empty(this->host))
    EXC_ILLEGAL_STATE("No request has been read")
  return opt_get(this->host);
}

char *iserverRq_write (IserverRq *this, char *response) {
  if (this->sock < 0 || opt_is_empty(this->msg))
    return "No request has been read";

  int sock = this->sock;
  char *p = response;
  int len = strlen(response) + 1;
  int nbytes;
  while ((nbytes = write(sock, p, len)) != 0) {
    if (nbytes < 0 && errno == EINTR) {
      return "Error 'EINTR' writing response";
    }
    p += nbytes;
    len -= nbytes;
  }
  fsync(sock);
  close(sock);
  return "";
}

struct iserver_Iserver {
  int sock;
  fd_set *active_fd_set;
};

Iserver *iserver_new (Gc *gc, int port) {
  int sock;
  struct sockaddr_in name;

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    EXC_IO("Fail creating socket");

  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0)
    EXC_IO("Fail binding socket");

  fd_set *active_fd_set = gc_add(gc, malloc(sizeof(fd_set)));

  if (listen (sock, 1) < 0)
    EXC_IO("Fail listenning sock")

  FD_ZERO (active_fd_set);
  FD_SET (sock, active_fd_set);

  Iserver *this = gc_add(gc, malloc(sizeof(Iserver)));
  this->sock = sock;
  this->active_fd_set = active_fd_set;
  return this;
}

IserverRq *iserver_read (Gc *gc, Iserver *this) {
  int sock = this->sock;
  fd_set *read_fd_set = this->active_fd_set;

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  int rs = select (FD_SETSIZE, read_fd_set, NULL, NULL, &timeout);

  if (rs < 0) {
    return iserverRq_new(gc, -1, opt_new("Fail in server select"), opt_empty());
  }
  if (!FD_ISSET(sock, read_fd_set)) {
    FD_SET (sock, read_fd_set);
    return iserverRq_new(gc, 0, opt_empty(), opt_empty());
  }

  struct sockaddr_in clientname;
  socklen_t size = sizeof(clientname);
  int new = accept(sock, (struct sockaddr *) &clientname, &size);
  if (new < 0) {
    return iserverRq_new(
      gc, -1, opt_new("Fail accepting connection"), opt_empty()
    );
  }

  Gc *gcl = gc_new();
  Buf *bf = buf_new(gcl);
  int SIZE = 8192;
  char buffer[SIZE];
  int nbytes;
  for (;;) {
    nbytes = read(new, buffer, SIZE);
    if (nbytes < 0) {
      close(new);
      FD_CLR (new, read_fd_set);
      gc_free(gcl);
      return iserverRq_new(
        gc, -1, opt_new("Fail reading client connection"), opt_empty()
      );
    }

    if (nbytes == 0) break;

    buf_add_buf(bf, buffer, nbytes);
  }

  FD_CLR (new, read_fd_set);
  IserverRq *r =  iserverRq_new(gc,
    new, opt_new(buf_to_str(gc, bf)),
    opt_new(str_new(gc, inet_ntoa(clientname.sin_addr)))
  );

  gc_free(gcl);
  return r;
}

void iserver_close (Iserver *this) {
  if (close(this->sock) < 0)
    EXC_IO("Fail closing socket");
  FD_CLR (this->sock, this->active_fd_set);
}


