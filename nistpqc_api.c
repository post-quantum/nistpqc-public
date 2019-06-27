#include "nistpqc_api.h"
#include "nistpqc_api_internal.h"


DECLARE_CIPHER_EXPORTS(newhope512cca)   /* NewHope 512 CCA-KEM */
DECLARE_CIPHER_EXPORTS(kyber512)        /* Kyber 512 */
DECLARE_CIPHER_EXPORTS(ntrulpr761)      /* NTRU LPrime 761 */
DECLARE_CIPHER_EXPORTS(ntrukem443)      /* NTRU KEM 443 */
DECLARE_CIPHER_EXPORTS(sikep434)        /* SIKEp434 */
DECLARE_CIPHER_EXPORTS(sikep503)        /* SIKEp503 */
DECLARE_CIPHER_EXPORTS(sikep610)        /* SIKEp610 */
DECLARE_CIPHER_EXPORTS(sikep751)        /* SIKEp751 */
DECLARE_CIPHER_EXPORTS(lac128)          /* LAC128 */
DECLARE_CIPHER_EXPORTS(lac192)          /* LAC192 */
DECLARE_CIPHER_EXPORTS(lac256)          /* LAC256 */
DECLARE_CIPHER_EXPORTS(ledakem128n3)    /* LEDAkem 128 n_0=3 */
DECLARE_CIPHER_EXPORTS(ledakem192n3)    /* LEDAkem 192 n_0=3 */
DECLARE_CIPHER_EXPORTS(ledakem256n2)    /* LEDAkem 256 n_0=2 */

int nistpqc_init(nistpqc_t *nistpqc, nistpqc_cipher_t cipher)
{
    switch (cipher)
    {
        case NISTPQC_CIPHER_NEWHOPE512CCA:
            INIT_CIPHER_INTERFACE(newhope512cca);
            break;
        case NISTPQC_CIPHER_KYBER512:
            INIT_CIPHER_INTERFACE(kyber512);
            break;
        case NISTPQC_CIPHER_NTRULPR761:
            INIT_CIPHER_INTERFACE(ntrulpr761);
            break;
        case NISTPQC_CIPHER_NTRUKEM443:
            INIT_CIPHER_INTERFACE(ntrukem443);
            break;
        case NISTPQC_CIPHER_SIKEP434:
            INIT_CIPHER_INTERFACE(sikep434);
            break;
        case NISTPQC_CIPHER_SIKEP503:
            INIT_CIPHER_INTERFACE(sikep503);
            break;
        case NISTPQC_CIPHER_SIKEP610:
            INIT_CIPHER_INTERFACE(sikep610);
            break;
        case NISTPQC_CIPHER_SIKEP751:
            INIT_CIPHER_INTERFACE(sikep751);
            break;
        case NISTPQC_CIPHER_LAC128:
            INIT_CIPHER_INTERFACE(lac128);
            break;
        case NISTPQC_CIPHER_LAC192:
            INIT_CIPHER_INTERFACE(lac192);
            break;
        case NISTPQC_CIPHER_LAC256:
            INIT_CIPHER_INTERFACE(lac256);
            break;
         case NISTPQC_CIPHER_LEDAKEM128N3:
            INIT_CIPHER_INTERFACE(ledakem128n3);
            break;
         case NISTPQC_CIPHER_LEDAKEM192N3:
            INIT_CIPHER_INTERFACE(ledakem192n3);
            break;
         case NISTPQC_CIPHER_LEDAKEM256N2:
            INIT_CIPHER_INTERFACE(ledakem256n2);
            break;
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
