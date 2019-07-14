/**
 * defunc_pk_from_sk.c
 * 
 * This is a non-functional implementation of public-key
 * from private-key method.
 * 
 * Because such a method is not defined as part of the
 * required APIs of NIST PQC algorithms, a developer shall
 * implement such a function in the respective NIST PQC
 * algorithm.
 */

#include <stdio.h>
#include "api.h"
#include "aux_api.h"

__attribute__((weak))
int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    fprintf(stderr, "This is a non-functional method of %s, ", __func__);
    fprintf(stderr, "it needs to be overridden with a real implementation.\n");
    
    return -1;
}
