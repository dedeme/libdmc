// Copyright 05-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/Iserver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>

struct iserver_Rq {
  int sock;
  Ochar *msg;
  Ochar *host;
};

static IserverRq *iserverRq_new (int sock, Ochar *msg, Ochar *host) {
  IserverRq *this = MALLOC(IserverRq);
  this->sock = sock;
  this->msg = msg;
  this->host = host;
  return this;
}

char *iserverRq_error (IserverRq *this) {
  if(this->sock < 0) {
    char *msg = ochar_nsome(this->msg);
    return msg ? msg : "Unknown error";
  }
  return "";
}

Ochar *iserverRq_msg (IserverRq *this) {
  if (this->sock < 0)
    FAIL("No request has been read");
  return this->msg;
}

char *iserverRq_host (IserverRq *this) {
  if (ochar_none(this->host))
    FAIL("No request has been read");
  return ochar_some(this->host);
}

char *iserverRq_write (IserverRq *this, char *response) {
  if (this->sock < 0 || ochar_none(this->msg))
    FAIL("No request has been read");

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
    FAIL("Fail creating socket");

  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  name->sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (struct sockaddr *) name, sizeof(*name)) < 0)
    FAIL("Fail binding socket");

  fd_set *active_fd_set = MALLOC(fd_set);

  if (listen (sock, 1) < 0)
    FAIL("Fail listenning sock");

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
    if (errno == EINTR)
      return iserverRq_new(0, ochar_mk_none(), ochar_mk_none());
    else
      return iserverRq_new(
        -1, ochar_mk_some("Fail in server select"), ochar_mk_none()
      );
  }
  if (!FD_ISSET(sock, read_fd_set)) {
    FD_SET (sock, read_fd_set);
    return iserverRq_new(0, ochar_mk_none(), ochar_mk_none());
  }

  struct sockaddr_in clientname;
  socklen_t size = sizeof(clientname);
  int new = accept(sock, (struct sockaddr *) &clientname, &size);
  if (new < 0) {
    return iserverRq_new(
      -1, ochar_mk_some("Fail accepting connection"), ochar_mk_none()
    );
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
      return iserverRq_new(
        -1, ochar_mk_some("Fail reading client connection"), ochar_mk_none()
      );
    }

    if (nbytes == 0) break;

    buf_add_buf(bf, buffer, nbytes);
  }

  FD_CLR (new, read_fd_set);
  return iserverRq_new(
    new, ochar_mk_some(buf_to_str(bf)),
    ochar_mk_some(str_new(inet_ntoa(clientname.sin_addr)))
  );
}

void iserver_close (Iserver *this) {
  if (close(this->sock) < 0)
    FAIL("Fail closing socket");
  FD_CLR (this->sock, this->active_fd_set);
}

