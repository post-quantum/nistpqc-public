#ifndef _NISTPQC_API_H
#define _NISTPQC_API_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
	NISTPQC_NEWHOPE512CCA = 1,
	NISTPQC_KYBER512,
	NISTPQC_NTRULPR4591761,
	NISTPQC_NTRUKEM443,
	NISTPQC_SIKEP503
} nistpqc_cipher_t;

typedef struct {
	int    (*keypair)(uint8_t *pk, uint8_t *sk);
	int    (*enc)(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
	int    (*dec)(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
	size_t (*shared_secret_size)(void);
	size_t (*ciphertext_size)(void);
	size_t (*public_key_size)(void);
	size_t (*private_key_size)(void);
	const char* (*algorithm_name)(void);
} nistpqc_t;

int nistpqc_init(nistpqc_t *nistpqc, nistpqc_cipher_t cipher);

#endif /* _NISTPQC_API_H */
