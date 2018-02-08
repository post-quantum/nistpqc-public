/**
 * rng.h
 *
 * OpenSSL wrapper of randombytes method
 **/

#ifndef __RNG_H
#define __RNG_H

#define RNG_SUCCESS      0
#define RNG_BAD_MAXLEN  -1
#define RNG_BAD_OUTBUF  -2
#define RNG_BAD_REQ_LEN -3

int
randombytes(unsigned char *x, unsigned long long xlen);

#endif /* __RNG_H */
