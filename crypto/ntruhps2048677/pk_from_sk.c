#include <string.h>
#include "poly.h"
#include "params.h"

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    poly h, invh;

    poly_Sq_frombytes(&invh, sk+2*NTRU_PACK_TRINARY_BYTES);
    poly_Rq_inv(&h, &invh);
    poly_Rq_sum_zero_tobytes(pk, &h);

    return 0;
}
