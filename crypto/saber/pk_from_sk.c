#include <string.h>
#include "SABER_params.h"

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    if (NULL != memcpy(pk,
                       sk + SABER_INDCPA_SECRETKEYBYTES,
                       SABER_PUBLICKEYBYTES))
        return 0;

    return -1;
}
