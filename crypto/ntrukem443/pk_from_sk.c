#include <string.h>
#include "param.h"
#include "api.h"

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    PARAM_SET   *param;

    param = get_param_set_by_id(sk[0]);
    if (NULL != memcpy(pk,
                       sk + param->packsk + 1,
                       CRYPTO_PUBLICKEYBYTES))
        return 0;

    return -1;
}
