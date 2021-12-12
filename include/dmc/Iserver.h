// Copyright 05-Dic-2021 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Inet server.
/// Example:
///   Iserver *is = iserver_new(8888);
///   for (;;) {
///     IserverRq *rq = iserver_read(is);
///     if (*iserverRq_error(rq))
///       FAIL(iserverRq_error(rq));
///
///     char *request = ochar_nsome(iserverRq_msg(rq));
///     if (request) {
///       if (str_eq(request, "end")) {
///         iserver_close(is);
///         return;
///       }
///       /**/void fn (void *rqOk) {
///       /**/  char *requestOk = ochar_some(iserverRq_msg(rqOk));
///       /**/  ... Process 'requestOk' and create 'response' ...
///       /**/  iserverRq_write(rqOk, response);
///       /**/}
///       async_thread2(fn, rq);
///     }
///     sys_sleep(200);
///   }

#ifndef DMC_ISERVER_H
  #define DMC_ISERVER_H

#include "dmc/char/Ochar.h"

/// Request read by Iserver.
typedef struct iserver_Rq IserverRq;

/// Returns a error message or an empty string if everithing was well.
char *iserverRq_error (IserverRq *this);

/// If no request was received, returns 'ochar_none'.
/// If 'iserverRq_error' returns an error, it raises a FAIL.
Ochar *iserverRq_msg (IserverRq *this);

/// Retuns the direction (IPv4) of connected server. It raises a FAIL
/// if no request was read.
/// For being sure about receiving request, 'iserverRq_msg' should be tested.
char *iserverRq_host (IserverRq *this);

/// Writes a response in 'this' and close it.
/// Returns an error message or an empty string if everithing was well.
char *iserverRq_write (IserverRq *this, char *response);

/// Internet server structure.
typedef struct iserver_Iserver Iserver;

///
Iserver *iserver_new (int port);

/// Read text in a not blocking way.
IserverRq *iserver_read (Iserver *this);

///
void iserver_close (Iserver *this);

#endif
