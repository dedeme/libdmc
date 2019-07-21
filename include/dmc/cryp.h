// Copyright 21-Jul-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Cryptographic utilities

#ifndef DMC_CRYP_H
  #define DMC_CRYP_H

#include "Gc.h"

/// Generates a b64 random key of a length 'lg'
char *cryp_genk (Gc *gc, int lg);

/// Codified 'k' in irreversible way, using 'lg' b64 digits.
///   gc  : Garbage collector
///   k   : String to codify
///   lg  : Length of result
char *cryp_key (Gc *gc, char *k, int lg);

/// Encodes 's' with key 'k'.
///   gc  : Garbage collector
///   s   : Message to encode
///   k   : Key for encoding
char *cryp_cryp (Gc *gc, char *s, char *k);

/// Decodes 'b64' using key 'k'. 'b64' was codified with cryp()
///   gc  : Garbage collector
///   b64: Text codified with cryp()
///   k   : Key for decoding
char *cryp_decryp (Gc *gc, char *b64, char *k);

/// Encodes automatically 's' with a random key of 'nk' digits.
///   gc  : Garbage collector
///   nK  : Number of digits for random key (1 to 64 both inclusive)
///   s   : Text for enconding
char *cryp_auto_cryp (Gc *gc, char *s, int nk);

/// Decodes a text codified with autoCryp()
///   gc  : Garbage collector
///   b64: Codified text
char *cryp_auto_decryp (Gc *gc, char *b64);

/// Encodes 's' whith key 'k' and an autoKey of length 'nK'
///   gc  : Garbage collector
///   k   : Key for encoding
///   nk  : Digits to generate autoKey (1 to 40 both inclusive)
///   s   : Message to encode
char *cryp_encode (Gc *gc, char *s, int nk, char *k);

/// Decodes a string codified with encode()
///   gc  : Garbage collector
///   b64: Message encoded with encode()
///   k   : Key for encoding
char *cryp_decode (Gc *gc, char *b64, char *k);

#endif
