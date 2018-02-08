#include "nistpqc_api.h"

/* NewHope 512 CCA-KEM */
extern int newhope512cca_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int newhope512cca_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int newhope512cca_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
extern size_t newhope512cca_crypto_get_shared_secret_size(void);
extern size_t newhope512cca_crypto_get_ciphertext_size(void);
extern size_t newhope512cca_crypto_get_public_key_size(void);
extern size_t newhope512cca_crypto_get_private_key_size(void);
extern const char* newhope512cca_crypto_get_algorithm_name(void);

/* Kyber 512 */
extern int kyber512_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int kyber512_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int kyber512_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
extern size_t kyber512_crypto_get_shared_secret_size(void);
extern size_t kyber512_crypto_get_ciphertext_size(void);
extern size_t kyber512_crypto_get_public_key_size(void);
extern size_t kyber512_crypto_get_private_key_size(void);
extern const char* kyber512_crypto_get_algorithm_name(void);

/* NTRU LPrime 4591^761 */
/* NTRU Prime naming is rather different */
extern int ntrulpr4591761_crypto_kem_ntrulpr4591761_ref_keypair(uint8_t *pk, uint8_t *sk);
extern int ntrulpr4591761_crypto_kem_ntrulpr4591761_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int ntrulpr4591761_crypto_kem_ntrulpr4591761_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
extern size_t ntrulpr4591761_crypto_get_shared_secret_size(void);
extern size_t ntrulpr4591761_crypto_get_ciphertext_size(void);
extern size_t ntrulpr4591761_crypto_get_public_key_size(void);
extern size_t ntrulpr4591761_crypto_get_private_key_size(void);
extern const char* ntrulpr4591761_crypto_get_algorithm_name(void);

/* NTRU KEM 443 */
extern int ntrukem443_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int ntrukem443_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int ntrukem443_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
extern size_t ntrukem443_crypto_get_shared_secret_size(void);
extern size_t ntrukem443_crypto_get_ciphertext_size(void);
extern size_t ntrukem443_crypto_get_public_key_size(void);
extern size_t ntrukem443_crypto_get_private_key_size(void);
extern const char* ntrukem443_crypto_get_algorithm_name(void);

/* SIKEp503 */
extern int sikep503_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
extern int sikep503_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int sikep503_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
extern size_t sikep503_crypto_get_shared_secret_size(void);
extern size_t sikep503_crypto_get_ciphertext_size(void);
extern size_t sikep503_crypto_get_public_key_size(void);
extern size_t sikep503_crypto_get_private_key_size(void);
extern const char* sikep503_crypto_get_algorithm_name(void);

int nistpqc_init(nistpqc_t *nistpqc, nistpqc_cipher_t cipher)
{
	switch (cipher)
	{
		case NISTPQC_NEWHOPE512CCA:
			nistpqc->keypair = &newhope512cca_crypto_kem_keypair;	
			nistpqc->enc = &newhope512cca_crypto_kem_enc;	
			nistpqc->dec = &newhope512cca_crypto_kem_dec;
			nistpqc->shared_secret_size = &newhope512cca_crypto_get_shared_secret_size;
			nistpqc->ciphertext_size = &newhope512cca_crypto_get_ciphertext_size;
			nistpqc->public_key_size = &newhope512cca_crypto_get_public_key_size;
			nistpqc->private_key_size = &newhope512cca_crypto_get_private_key_size;
			nistpqc->algorithm_name = &newhope512cca_crypto_get_algorithm_name;
			break;
		case NISTPQC_KYBER512:
			nistpqc->keypair = &kyber512_crypto_kem_keypair;	
			nistpqc->enc = &kyber512_crypto_kem_enc;	
			nistpqc->dec = &kyber512_crypto_kem_dec;
			nistpqc->shared_secret_size = &kyber512_crypto_get_shared_secret_size;
			nistpqc->ciphertext_size = &kyber512_crypto_get_ciphertext_size;
			nistpqc->public_key_size = &kyber512_crypto_get_public_key_size;
			nistpqc->private_key_size = &kyber512_crypto_get_private_key_size;
			nistpqc->algorithm_name = &kyber512_crypto_get_algorithm_name;
			break;
		case NISTPQC_NTRULPR4591761:
			nistpqc->keypair = &ntrulpr4591761_crypto_kem_ntrulpr4591761_ref_keypair;	
			nistpqc->enc = &ntrulpr4591761_crypto_kem_ntrulpr4591761_ref_enc;	
			nistpqc->dec = &ntrulpr4591761_crypto_kem_ntrulpr4591761_ref_dec;
			nistpqc->shared_secret_size = &ntrulpr4591761_crypto_get_shared_secret_size;
			nistpqc->ciphertext_size = &ntrulpr4591761_crypto_get_ciphertext_size;
			nistpqc->public_key_size = &ntrulpr4591761_crypto_get_public_key_size;
			nistpqc->private_key_size = &ntrulpr4591761_crypto_get_private_key_size;
			nistpqc->algorithm_name = &ntrulpr4591761_crypto_get_algorithm_name;
			break;
		case NISTPQC_NTRUKEM443:
			nistpqc->keypair = &ntrukem443_crypto_kem_keypair;	
			nistpqc->enc = &ntrukem443_crypto_kem_enc;	
			nistpqc->dec = &ntrukem443_crypto_kem_dec;
			nistpqc->shared_secret_size = &ntrukem443_crypto_get_shared_secret_size;
			nistpqc->ciphertext_size = &ntrukem443_crypto_get_ciphertext_size;
			nistpqc->public_key_size = &ntrukem443_crypto_get_public_key_size;
			nistpqc->private_key_size = &ntrukem443_crypto_get_private_key_size;
			nistpqc->algorithm_name = &ntrukem443_crypto_get_algorithm_name;
			break;
		case NISTPQC_SIKEP503:
			nistpqc->keypair = &sikep503_crypto_kem_keypair;	
			nistpqc->enc = &sikep503_crypto_kem_enc;	
			nistpqc->dec = &sikep503_crypto_kem_dec;
			nistpqc->shared_secret_size = &sikep503_crypto_get_shared_secret_size;
			nistpqc->ciphertext_size = &sikep503_crypto_get_ciphertext_size;
			nistpqc->public_key_size = &sikep503_crypto_get_public_key_size;
			nistpqc->private_key_size = &sikep503_crypto_get_private_key_size;
			nistpqc->algorithm_name = &sikep503_crypto_get_algorithm_name;
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
			return 0;	/* Unknown cipher, return FALSE */
	}

	return 1;
}

