#include <string.h>
#include "api.h"

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    if (NULL != memcpy(pk,
                       sk + CRYPTO_BYTES,
                       CRYPTO_PUBLICKEYBYTES))
        return 0;

    return -1;
}
