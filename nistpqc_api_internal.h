
// A couple of helper declarations to keep nistpqc_api.c clean

#define DECLARE_CIPHER_EXPORTS(x) \
  extern int x##_crypto_kem_keypair(uint8_t *pk, uint8_t *sk); \
  extern int x##_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk); \
  extern int x##_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk); \
  extern size_t x##_crypto_get_shared_secret_size(void); \
  extern size_t x##_crypto_get_ciphertext_size(void); \
  extern size_t x##_crypto_get_public_key_size(void); \
  extern size_t x##_crypto_get_private_key_size(void); \
  extern const char* x##_crypto_get_algorithm_name(void); \
  extern int x##_crypto_public_key_from_private_key(uint8_t*, const uint8_t*);

#define INIT_CIPHER_INTERFACE(x) \
    nistpqc->keypair = &x##_crypto_kem_keypair; \
    nistpqc->enc = &x##_crypto_kem_enc; \
    nistpqc->dec = &x##_crypto_kem_dec; \
    nistpqc->shared_secret_size = &x##_crypto_get_shared_secret_size; \
    nistpqc->ciphertext_size = &x##_crypto_get_ciphertext_size; \
    nistpqc->public_key_size = &x##_crypto_get_public_key_size; \
    nistpqc->private_key_size = &x##_crypto_get_private_key_size; \
    nistpqc->algorithm_name = &x##_crypto_get_algorithm_name; \
    nistpqc->public_key_from_private_key = &x##_crypto_public_key_from_private_key;
