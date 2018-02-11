// Copyright 05-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Cryptographic utilities

#ifndef DM_CRYP_H
  #define DM_CRYP_H

/// Generates a b64 random key of a length 'lg'
char *cryp_genK (int lg);

/// Returns 'k' codified in irreversible way, using 'lg' b64 digits.
///   k   : String to codify
///   lg  : Length of result
char *cryp_key (char *k, int lg);

/// Encodes 's' with key 'k'.
///   k   : Key for encoding
///   s   : Message to encode
char *cryp_cryp (char *k, char *s);

/// Decodes 'b64' using key 'k'. 'b64' was codified with cryp()
///   k   : Key for decoding
///   b64: Text codified with cryp()
char *cryp_decryp (char *k, char *b64);

/// Encodes automatically 's' with a random key of 'nk' digits.
///   nK  : Number of digits for random key (1 to 64 both inclusive)
///   s   : Text for enconding
char *cryp_auto_cryp (int nK, char *s);

/// Decodes a text codified with autoCryp()
///   b64: Codified text
char *cryp_auto_decryp (char *b64);

/// Encodes 's' whith key 'k' and an autoKey of length 'nK'
///   k   : Key for encoding
///   mK  : Digits to generate autoKey (1 to 40 both inclusive)
///   s   : Message to encode
char *cryp_encode (char *k, int nK, char *s);

/// Decodes a string codified with encode()
///   k   : Key for encoding
///   b64: Message encoded with encode()
char *cryp_decode (char *k, char *b64);

#endif

