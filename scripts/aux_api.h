#ifndef _AUX_API_H
#define _AUX_API_H

#include <stddef.h>

/**
 * Return the shared-secret size in bytes
 *
 * @return the number of bytes the shared-secret consumes
 **/
size_t crypto_get_shared_secret_size(void);

/**
 * Return the ciphertext size in bytes
 *
 * @return the number of bytes the ciphertext consumes
 **/
size_t crypto_get_ciphertext_size(void);

/**
 * Return the public-key size in bytes
 *
 * @return the number of bytes the public-key consumes
 **/
size_t crypto_get_public_key_size(void);

/**
 * Return the private-key size in bytes
 *
 * @return the number of bytes the private-key consumes
 **/
size_t crypto_get_private_key_size(void);

/**
 * Return the name of the algorithm
 *
 * @return the algorithm name
 **/
const char* crypto_get_algorithm_name(void);

#endif /* _AUX_API_H */
