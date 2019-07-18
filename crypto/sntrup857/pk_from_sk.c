#include <string.h>
#include "params.h"

#define p 857
#define Small_bytes ((p+3)/4)
#define Rq_bytes 1322
#define SecretKeys_bytes (2*Small_bytes)
#define PublicKeys_bytes Rq_bytes

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    if (NULL != memcpy(pk,
                       sk + SecretKeys_bytes,
                       PublicKeys_bytes))
        return 0;

    return -1;
}
