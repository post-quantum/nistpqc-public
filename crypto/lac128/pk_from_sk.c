#include <string.h>
#include "lac_param.h"

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    if (NULL != memcpy(pk,
                       sk + DIM_N,
                       PK_LEN))
        return 0;

    return -1;
}
