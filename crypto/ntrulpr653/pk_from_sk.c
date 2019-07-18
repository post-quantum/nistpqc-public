#include <string.h>
#include "params.h"

#define p 653
#define Seeds_bytes 32
#define Small_bytes ((p+3)/4)
#define Rounded_bytes 865
#define SecretKeys_bytes Small_bytes
#define PublicKeys_bytes (Seeds_bytes+Rounded_bytes)

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    if (NULL != memcpy(pk,
                       sk + SecretKeys_bytes,
                       PublicKeys_bytes))
        return 0;

    return -1;
}
