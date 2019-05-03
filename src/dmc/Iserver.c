// Copyright 01-May-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Iserver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

struct iserver_Rq {
  int sock;
  // Opt[char]
  Opt *msg;
};

// msg is Opt[char]
static IserverRq *iserverRq_new (int sock, Opt *msg) {
  IserverRq *this = MALLOC(IserverRq);
  this->sock = sock;
  this->msg = msg;
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

char *iserverRq_write (IserverRq *this, char *response) {
  if (this->sock < 0 || opt_is_empty(this->msg))
    EXC_ILLEGAL_STATE("No request has been read")

  int sock = this->sock;
  char *p = response;
  int len = strlen(response) + 1;
  int nbytes;
  while ((nbytes = write(sock, p, len)) != 0) {
    if (nbytes < 0 && errno == EINTR) {
      return str_f("Error '%s' writing response", strerror(errno));
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

Iserver *iserver_new (int port) {
  int sock;
  struct sockaddr_in *name = MALLOC(struct sockaddr_in);

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    EXC_IO("Fail creating socket");

  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  name->sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (struct sockaddr *) name, sizeof(*name)) < 0)
    EXC_IO("Fail binding socket");

  fd_set *active_fd_set = MALLOC(fd_set);

  if (listen (sock, 1) < 0)
    EXC_IO("Fail listenning sock")

  FD_ZERO (active_fd_set);
  FD_SET (sock, active_fd_set);

  Iserver *this = MALLOC(Iserver);
  this->sock = sock;
  this->active_fd_set = active_fd_set;
  return this;
}

IserverRq *iserver_read (Iserver *this) {
  int sock = this->sock;
  fd_set *read_fd_set = this->active_fd_set;

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  int rs = select (FD_SETSIZE, read_fd_set, NULL, NULL, &timeout);

  if (rs < 0) {
    return iserverRq_new(-1, opt_new("Fail in server select"));
  }
  if (!FD_ISSET(sock, read_fd_set)) {
    FD_SET (sock, read_fd_set);
    return iserverRq_new(0, opt_empty());
  }

  struct sockaddr_in clientname;
  socklen_t size = sizeof(clientname);
  int new = accept(sock, (struct sockaddr *) &clientname, &size);
  if (new < 0) {
    return iserverRq_new(-1, opt_new("Fail accepting connection"));
  }

  Buf *bf = buf_new();
  int SIZE = 8192;
  char buffer[SIZE];
  int nbytes;
  for (;;) {
    nbytes = read(new, buffer, SIZE);
    if (nbytes < 0) {
      close(new);
      FD_CLR (new, read_fd_set);
      return iserverRq_new(-1, opt_new("Fail reading client connection"));
    }

    if (nbytes == 0) break;

    buf_add_buf(bf, buffer, nbytes);
  }

  FD_CLR (new, read_fd_set);
  return iserverRq_new(new, opt_new(buf_to_str(bf)));
}

void iserver_close (Iserver *this) {
  if (close(this->sock) < 0)
    EXC_IO("Fail closing socket");
  FD_CLR (this->sock, this->active_fd_set);
}

