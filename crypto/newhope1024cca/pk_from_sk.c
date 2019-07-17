#include <string.h>
#include "params.h"

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    if (NULL != memcpy(pk,
                       sk + NEWHOPE_CPAPKE_SECRETKEYBYTES,
                       NEWHOPE_CPAPKE_PUBLICKEYBYTES))
        return 0;

    return -1;
}
