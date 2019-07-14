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

/**
 * Create a public-key from a private-key.
 * 
 * The size of the private-key can be obtained by calling
 * crypto_get_private_key_size method, and the size of the
 * public-key from crpyto_get_public_key_size method.
 * 
 * The output buffer pk must be pre-allocated.
 * 
 * @param pk Pointer to the output public-key
 * @param sk Pointer to the input private-key
 * @return 0 on success, non-zero otherwise
 */
int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk);

#endif /* _AUX_API_H */
