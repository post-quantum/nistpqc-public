#include <string.h>
#include "api.h"
#include "poly.h"

/* n^-1 modulo q */
#define INV_N_MOD_Q         2197

int crypto_public_key_from_private_key(unsigned char* pk,
                                       const unsigned char* sk)
{
    int i;
    poly h, hq;
    uint32_t d, sum = 0;

    memset(hq.coeffs, 0, sizeof(hq.coeffs));
    poly_Sq_frombytes(&hq, sk+2*NTRU_PACK_TRINARY_BYTES);

    memset(h.coeffs, 0, sizeof(h.coeffs));
    poly_Rq_inv(&h, &hq);

    /* Compute the modulo-q sum of the coefficients of h */
    for (i=0; i<NTRU_N; i++) sum += h.coeffs[i];

    /* Update the coefficients of h so that sum (h_i - d)/n = 0 mod q */
    d = MODQ(INV_N_MOD_Q * sum);
    for (i=0; i<NTRU_N; i++) h.coeffs[i] = MODQ(h.coeffs[i] - d);

    poly_Rq_sum_zero_tobytes(pk, &h);

    return 0;
}
