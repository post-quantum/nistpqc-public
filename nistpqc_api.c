#include "nistpqc_api.h"
#include "nistpqc_api_internal.h"


DECLARE_CIPHER_EXPORTS(sikep434)        /* SIKEp434 */
DECLARE_CIPHER_EXPORTS(sikep503)        /* SIKEp503 */
DECLARE_CIPHER_EXPORTS(sikep610)        /* SIKEp610 */
DECLARE_CIPHER_EXPORTS(sikep751)        /* SIKEp751 */
DECLARE_CIPHER_EXPORTS(frodokem640)     /* FrodoKEM-640 */
DECLARE_CIPHER_EXPORTS(frodokem976)     /* FrodoKEM-976 */
DECLARE_CIPHER_EXPORTS(frodokem1344)    /* FrodoKEM-1344 */
DECLARE_CIPHER_EXPORTS(ntruhps2048509)  /* NTRU-hps 2048 509 */
DECLARE_CIPHER_EXPORTS(ntruhps2048677)  /* NTRU-hps 2048 677 */
DECLARE_CIPHER_EXPORTS(ntruhps4096821)  /* NTRU-hps 4096 821 */
DECLARE_CIPHER_EXPORTS(ntruhrss701)     /* NTRU-hrss 701 */
DECLARE_CIPHER_EXPORTS(kyber512)        /* Kyber 512 */
DECLARE_CIPHER_EXPORTS(kyber768)        /* Kyber 768 */
DECLARE_CIPHER_EXPORTS(kyber1024)       /* Kyber 1024 */
DECLARE_CIPHER_EXPORTS(ntrulpr653)      /* NTRU LPrime 653 */
DECLARE_CIPHER_EXPORTS(ntrulpr761)      /* NTRU LPrime 761 */
DECLARE_CIPHER_EXPORTS(ntrulpr857)      /* NTRU LPrime 857 */
DECLARE_CIPHER_EXPORTS(sntrup653)       /* Streamlined NTRU Prime 653 */
DECLARE_CIPHER_EXPORTS(sntrup761)       /* Streamlined NTRU Prime 761 */
DECLARE_CIPHER_EXPORTS(sntrup857)       /* Streamlined NTRU Prime 857 */
DECLARE_CIPHER_EXPORTS(lightsaber)      /* LightSaber */
DECLARE_CIPHER_EXPORTS(saber)           /* Saber */
DECLARE_CIPHER_EXPORTS(firesaber)       /* FireSaber */

#define case_(x,y) case NISTPQC_CIPHER_##x: INIT_CIPHER_INTERFACE(y); break

int nistpqc_init(nistpqc_t *nistpqc, nistpqc_cipher_t cipher)
{


    nistpqc->public_key_from_private_key = NULL;
    switch (cipher)
    {
        case_(SIKEP434, sikep434);
        case_(SIKEP503, sikep503);
        case_(SIKEP610, sikep610);
        case_(SIKEP751, sikep751);
        case_(FRODOKEM640, frodokem640);
        case_(FRODOKEM976, frodokem976);
        case_(FRODOKEM1344, frodokem1344);
        case_(NTRUHPS2048509, ntruhps2048509);
        case_(NTRUHPS2048677, ntruhps2048677);
        case_(NTRUHPS4096821, ntruhps4096821);
        case_(NTRUHRSS701, ntruhrss701);
        case_(KYBER512, kyber512);
        case_(KYBER768, kyber768);
        case_(KYBER1024, kyber1024);
        case_(NTRULPR653, ntrulpr653);
        case_(NTRULPR761, ntrulpr761);
        case_(NTRULPR857, ntrulpr857);
        case_(SNTRUP653, sntrup653);
        case_(SNTRUP761, sntrup761);
        case_(SNTRUP857, sntrup857);
    	case_(LIGHTSABER, lightsaber);
        case_(SABER, saber);
        case_(FIRESABER, firesaber);
        default:
            nistpqc->keypair = NULL;
            nistpqc->enc = NULL;
            nistpqc->dec = NULL;
            nistpqc->shared_secret_size = NULL;
            nistpqc->ciphertext_size = NULL;
            nistpqc->public_key_size = NULL;
            nistpqc->private_key_size = NULL;
            nistpqc->algorithm_name = NULL;
            return 0;   /* Unknown cipher, return FALSE */
    }

    nistpqc->cipher_id = cipher;
    return 1;
}
