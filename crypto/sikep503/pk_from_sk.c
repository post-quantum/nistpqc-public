#include <string.h>
#include "api.h"
#include "P503/P503_internal.h"

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    if (NULL != memcpy(pk,
                       sk + MSG_BYTES + SECRETKEY_B_BYTES,
                       CRYPTO_PUBLICKEYBYTES))
        return 0;

    return -1;
}
