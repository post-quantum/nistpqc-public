#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poly.h"
#include "api.h"
#include "rng.h"

extern int crypto_public_key_from_private_key(unsigned char *pk, const unsigned char *sk);

int main() {
    unsigned char pk0[CRYPTO_PUBLICKEYBYTES];
    unsigned char pk [CRYPTO_PUBLICKEYBYTES];
    unsigned char sk [CRYPTO_SECRETKEYBYTES];
    unsigned char k  [CRYPTO_BYTES];
    unsigned char ss [CRYPTO_BYTES];
    unsigned char _ss[CRYPTO_BYTES];
    unsigned char ct [CRYPTO_CIPHERTEXTBYTES];

    memset(pk0, 0, sizeof(pk0));
    memset(pk,  0, sizeof(pk));
    memset(sk,  0, sizeof(sk));
    memset(k,   0, sizeof(k));
    memset(ss,  0, sizeof(ss));
    memset(_ss, 0, sizeof(_ss));
    memset(ct,  0, sizeof(ct));

    randombytes(k, CRYPTO_BYTES);

    if (0 != crypto_kem_keypair(pk, sk)) {
        printf("crypto_kem_keypair FAILED\n");
        return -1;
    }

    if (0 != crypto_kem_enc(ct, ss, pk)) {
        printf("crypto_kem_enc FAILED\n");
        return -1;
    }
    
    if (0 != crypto_kem_dec(_ss, ct, sk)) {
        printf("crypto_kem_enc FAILED\n");
        return -1;
    }

    printf("1 : %s\n", 0 == memcmp(_ss, ss, sizeof(ss)) ? "SUCCESS" : "FAILED");

    memset(ss,  0, sizeof(ss));
    memset(_ss, 0, sizeof(_ss));
    memset(ct,  0, sizeof(ct));

    crypto_public_key_from_private_key(pk0, sk);

    if (0 != crypto_kem_enc(ct, ss, pk0)) {
        printf("crypto_kem_enc FAILED\n");
        return -1;
    }
    
    if (0 != crypto_kem_dec(_ss, ct, sk)) {
        printf("crypto_kem_enc FAILED\n");
        return -1;
    }

    printf("2 : %s\n", 0 == memcmp(_ss, ss, sizeof(ss)) ? "SUCCESS" : "FAILED");
    
    return 0;
}
