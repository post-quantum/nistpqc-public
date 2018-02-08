#ifndef crypto_stream_aes256ctr_H
#define crypto_stream_aes256ctr_H

#define crypto_stream_aes256ctr_openssl_KEYBYTES 32
#define crypto_stream_aes256ctr_openssl_NONCEBYTES 16
 
#ifdef __cplusplus
extern "C" {
#endif
extern int crypto_stream_aes256ctr_openssl(unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
extern int crypto_stream_aes256ctr_openssl_xor(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_stream_aes256ctr crypto_stream_aes256ctr_openssl
#define crypto_stream_aes256ctr_xor crypto_stream_aes256ctr_openssl_xor
#define crypto_stream_aes256ctr_KEYBYTES crypto_stream_aes256ctr_openssl_KEYBYTES
#define crypto_stream_aes256ctr_NONCEBYTES crypto_stream_aes256ctr_openssl_NONCEBYTES
#define crypto_stream_aes256ctr_IMPLEMENTATION "crypto_stream/aes256ctr/openssl"
#ifndef crypto_stream_aes256ctr_openssl_VERSION
#define crypto_stream_aes256ctr_openssl_VERSION "-"
#endif
#define crypto_stream_aes256ctr_VERSION crypto_stream_aes256ctr_openssl_VERSION

#endif
