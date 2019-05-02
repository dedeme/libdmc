// Copyright 01-May-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Inet server.

#ifndef DMC_ISERVER_H
  #define DMC_ISERVER_H

#include "dmc/std.h"

/// Request read by Iserver
typedef struct iserver_Rq IserverRq;

/// Returns a error message or an empty string if everithing was well.
char *iserverRq_error (IserverRq *this);

/// Returns Opt[char]. If no request was received, returns 'opt_emtpy'.
char *iserverRq_error (IserverRq *this);

/// Returns Opt[char]. If no request was received, returns 'opt_emtpy'.
/// If 'iserverRq_error' returns an error, throws an ILLEGAL_STATE exception.
Opt *iserverRq_msg (IserverRq *this);

/// Returns a error message or an empty string if everithing was well.
char *iserverRq_write (IserverRq *this, char *response);

/// Writes response in 'this' and close it.
///   Returns a error message or an empty string if everithing was well.
char *iserverRq_write (IserverRq *this, char *response);

///
typedef struct iserver_Iserver Iserver;

///
Iserver *iserver_new (int port);

/// Read text in a not blocking way.
IserverRq *iserver_read (Iserver *this);

///
void iserver_close (Iserver *this);

#endif
