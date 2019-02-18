//
// Copyright © 2019 Post-Quantum Solutions Ltd. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <openssl/engine.h>
#include "../nistpqc_api.h"
#include "nistpqc_openssl.h"


// Insist on OpenSSL 1.1.0 or later
#if OPENSSL_VERSION_NUMBER < 0x10100000
#error Requires OpenSSL 1.1 or later 
#endif

// List of supported NIDs. 
static const int meth_nids[] = {
    NID_NISTPQC_NEWHOPE512CCA,
    NID_NISTPQC_KYBER512, 
    NID_NISTPQC_NTRULPR4591761,
    NID_NISTPQC_NTRUKEM443,
    NID_NISTPQC_SIKEP503,
    NID_NISTPQC_LEDAKEM128SLN02,
    0
};

// Key data struct. An EVP_PKEY holds a pointer to one of these.
typedef struct {
    size_t pk_size;
    size_t sk_size;
    uint8_t* pubkey;
    uint8_t* privkey;
} nistpqc_keyinfo_t;


// Mapping between cipher IDs and OpenSSL NIDs

static int NIDfromCipherID(nistpqc_cipher_t cipher_id) {
    switch (cipher_id) {
    case NISTPQC_CIPHER_NEWHOPE512CCA:  return NID_NISTPQC_NEWHOPE512CCA; 
    case NISTPQC_CIPHER_KYBER512:       return NID_NISTPQC_KYBER512; 
    case NISTPQC_CIPHER_NTRULPR4591761: return NID_NISTPQC_NTRULPR4591761;
    case NISTPQC_CIPHER_NTRUKEM443:     return NID_NISTPQC_NTRUKEM443;
    case NISTPQC_CIPHER_SIKEP503:       return NID_NISTPQC_SIKEP503;
    case NISTPQC_CIPHER_LEDAKEM128SLN02:return NID_NISTPQC_LEDAKEM128SLN02;
    }
    return 0;
}
static nistpqc_cipher_t CipherIDFromNID(int nid) {
    switch (nid) {
    case NID_NISTPQC_NEWHOPE512CCA:   return NISTPQC_CIPHER_NEWHOPE512CCA;
    case NID_NISTPQC_KYBER512:        return NISTPQC_CIPHER_KYBER512;
    case NID_NISTPQC_NTRULPR4591761:  return NISTPQC_CIPHER_NTRULPR4591761;
    case NID_NISTPQC_NTRUKEM443:      return NISTPQC_CIPHER_NTRUKEM443;
    case NID_NISTPQC_SIKEP503:        return NISTPQC_CIPHER_SIKEP503;
    case NID_NISTPQC_LEDAKEM128SLN02: return NISTPQC_CIPHER_LEDAKEM128SLN02;
    }
    return 0;
}



/** 
 * EVP_PKEY_CTX initialization - instantiate the desired cipher 
 * interface and bind it to the context.
 */
static int pkey_ctx_init(EVP_PKEY_CTX* ctx) {

    // Get the NID for the algorithm we want
    EVP_PKEY_METHOD* pmeth = *(EVP_PKEY_METHOD**)ctx; // dodgy but convenient
    int nid = 0;
    EVP_PKEY_meth_get0_info(&nid, NULL, pmeth);    

    // Map the NID to the nistpqc cipher ID
    nistpqc_cipher_t cipher_id = CipherIDFromNID(nid);

    // Instantiate the cipher interface
    nistpqc_t* cipher = OPENSSL_zalloc(sizeof(nistpqc_t));
    if (!cipher) {
        // todo: report error
        return 0;
    }
    nistpqc_init(cipher, cipher_id);

    // Store the cipher interface in the key context
    EVP_PKEY_CTX_set_data(ctx, cipher);
    return 1;
}
static void pkey_ctx_cleanup(EVP_PKEY_CTX* ctx) {
    nistpqc_t* cipher = EVP_PKEY_CTX_get_data(ctx);
    if (cipher) {
        OPENSSL_free(cipher);
        EVP_PKEY_CTX_set_data(ctx, NULL);
    }
}

/**
 * Keypair generation callback. Note that OpenSSL's EVP API presupposes that both
 * sides generate keypairs, whereas in KEM only the client does so. The server
 * instead generates the shared secret and stores that as it's "private key"
 * and then encapsulates the secret using the peer public key and considers that
 * as it's "public key" as sent in the ServerHello keyshare.
 */
static int pkey_keygen(EVP_PKEY_CTX *ctx, EVP_PKEY *pkey) {
    
    if (pkey == NULL || ctx == NULL) {
        return 0;
    }

    nistpqc_t* cipher = EVP_PKEY_CTX_get_data(ctx);

    nistpqc_keyinfo_t* keyinfo =  OPENSSL_zalloc(sizeof(nistpqc_keyinfo_t));
    if (!keyinfo) {
        return 0;
    }

    int nid = NIDfromCipherID(cipher->cipher_id);
    EVP_PKEY_assign(pkey, nid, keyinfo);


    EVP_PKEY* peerKey = EVP_PKEY_CTX_get0_pkey(ctx);

    // Client KEM: generate a keypair
    if (!EVP_PKEY_CTX_get_server(ctx)) {

        // Allocations to hold the keypair
        keyinfo->pk_size = cipher->public_key_size();
        keyinfo->sk_size = cipher->private_key_size();
        keyinfo->pubkey = OPENSSL_malloc(keyinfo->pk_size);
        keyinfo->privkey = OPENSSL_secure_malloc(keyinfo->sk_size);

        // Generate the keypair
        if (0 != cipher->keypair(keyinfo->pubkey, keyinfo->privkey)) {
            // todo: errors
            return 0;
        }

    // Server KEM: Generate the shared secret and pretend its our private key
    } else {
        size_t shared_secret_size = cipher->shared_secret_size();
        unsigned char* shared_secret = OPENSSL_secure_malloc(shared_secret_size);
        RAND_bytes(shared_secret, shared_secret_size);
        keyinfo->sk_size = shared_secret_size;
        keyinfo->privkey = shared_secret;

        // Encapsulate the shared secret using the peer public key and that's our
        // "public key" that goes into the keyshare extension sent back to the client.
        nistpqc_keyinfo_t* peerKeyinfo = (nistpqc_keyinfo_t*)EVP_PKEY_get0(peerKey);
        keyinfo->pk_size = cipher->ciphertext_size();
        keyinfo->pubkey = OPENSSL_malloc(keyinfo->pk_size);
        if (0 != cipher->enc(keyinfo->pubkey, shared_secret, peerKeyinfo->pubkey)) {
            // todo: errors
            return 0;
        }
    }
    
    // Success!
    return 1;
}

static int pkey_ctrl(EVP_PKEY_CTX *ctx, int type, int p1, void *p2) {
    // By handling this we indicate to OpenSSL runtime that we need the peer key for derivation
    if (type == EVP_PKEY_CTRL_PEER_KEY)
        return 1;
    return -2;
}
static int pkey_derive(EVP_PKEY_CTX *ctx, unsigned char *key,size_t *keylen) {
    EVP_PKEY* pkey = EVP_PKEY_CTX_get0_pkey(ctx);
    nistpqc_keyinfo_t* keyinfo = (nistpqc_keyinfo_t*)EVP_PKEY_get0(pkey);

    // Server: no derivation needed, the shared secret is our private key data
    if (EVP_PKEY_CTX_get_server(ctx)) {
        *keylen = keyinfo->sk_size;
        if (key) {
            memcpy(key, keyinfo->privkey, keyinfo->sk_size);
        }
    }

    // Client: decapsulate the server's "public key" to get the shared secret
    else {
        nistpqc_t* cipher = EVP_PKEY_CTX_get_data(ctx);
        *keylen = cipher->shared_secret_size();
        if (key) {
            EVP_PKEY* peerKey = EVP_PKEY_CTX_get0_peerkey(ctx);
            nistpqc_keyinfo_t* peerKeyinfo = (nistpqc_keyinfo_t*)EVP_PKEY_get0(peerKey);
            cipher->dec(key, peerKeyinfo->pubkey, keyinfo->privkey);
        }
    }

    // DEBUG ONLY!
    //if (key) {
    //    printf("*** SECRET! %02X%02X%02X%02X%02X%02X%02X%02X...\n", key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7]);
    //}
    return *keylen;
}


static int pkey_meths(ENGINE *e, EVP_PKEY_METHOD **pmeth, const int **nids, int nid) {
    if (!pmeth) {
        *nids = meth_nids;
        return sizeof(meth_nids) / sizeof(meth_nids[0]) - 1;
    }

    *pmeth = EVP_PKEY_meth_new(nid, EVP_PKEY_FLAG_SIGCTX_CUSTOM);
    EVP_PKEY_meth_set_init(*pmeth, pkey_ctx_init);
    EVP_PKEY_meth_set_cleanup(*pmeth, pkey_ctx_cleanup);
    EVP_PKEY_meth_set_keygen(*pmeth, NULL, pkey_keygen);
    EVP_PKEY_meth_set_derive(*pmeth, NULL, pkey_derive);
    EVP_PKEY_meth_set_ctrl(*pmeth, pkey_ctrl, NULL);
    return 1;
}

static int pkey_asn_ctrl(EVP_PKEY *pkey, int op, long arg1, void *arg2) {
    nistpqc_keyinfo_t* keyinfo = EVP_PKEY_get0(pkey);
    if (op == ASN1_PKEY_CTRL_GET1_TLS_ENCPT) {
        unsigned char **ppt = arg2;
        // Just copy public key 
        *ppt = OPENSSL_memdup(keyinfo->pubkey, keyinfo->pk_size);
        return (*ppt) ? keyinfo->pk_size : 0;
    }
    if (op == ASN1_PKEY_CTRL_SET1_TLS_ENCPT) {
        if (!keyinfo) {
            keyinfo = OPENSSL_zalloc(sizeof(nistpqc_keyinfo_t));
            EVP_PKEY_assign(pkey, EVP_PKEY_id(pkey), keyinfo);
        }
        keyinfo->pk_size = arg1;
        keyinfo->pubkey = OPENSSL_memdup(arg2, arg1);
        return arg1;
    }
    printf("todo: nistpqc_pkey_ctrl %d\n", op);
    return 0;
}
static int pub_decode(EVP_PKEY *pk, X509_PUBKEY *pub) {
    printf("todo: nistpqc_pub_decode\n");
    return 0;
}
static int pub_encode(X509_PUBKEY *pub, const EVP_PKEY *pk) {
    printf("todo: nistpqc_pub_encode\n");
    return 0;
}
static int pub_cmp(const EVP_PKEY *a, const EVP_PKEY *b) {
    printf("todo: nistpqc_pub_cmp\n");
    return 0;
}
static int pub_print(BIO *out, const EVP_PKEY *pkey, int indent, ASN1_PCTX *pctx) {
    printf("todo: nistpqc_pub_print\n");
    return 0;
}
static int pkey_size(const EVP_PKEY *pk) {
    printf("todo: nistpqc_pkey_size\n");
    return 0;
}
static int pkey_bits(const EVP_PKEY *pk) {
    printf("todo: nistpqc_pkey_bits\n");
    return 0;
}
static int priv_decode(EVP_PKEY *pk, const PKCS8_PRIV_KEY_INFO *p8inf) {
    printf("todo: nistpqc_priv_decode\n");
    return 0;
}
static int priv_encode(PKCS8_PRIV_KEY_INFO *p8, const EVP_PKEY *pk) {
    printf("todo: nistpqc_priv_encode\n");
    return 0;
}
static int priv_print(BIO *out, const EVP_PKEY *pkey, int indent, ASN1_PCTX *pctx) {
    printf("todo: nistpqc_priv_print\n");
    return 0;
}
static int pkey_security_bits(const EVP_PKEY *pk) {
    printf("todo: nistpqc_pkey_security_bits\n");
    return 0;
}
static void pkey_free(EVP_PKEY* pkey) {
     nistpqc_keyinfo_t* keyinfo =  EVP_PKEY_get0(pkey);
     if (keyinfo) {
         if (keyinfo->privkey) {
             OPENSSL_secure_free(keyinfo->privkey);
         }
         if (keyinfo->pubkey) {
             OPENSSL_free(keyinfo->pubkey);
         }
        OPENSSL_free(keyinfo);
        EVP_PKEY_assign(pkey, EVP_PKEY_id(pkey), NULL);
     }
}

static int pkey_asn1_meths(ENGINE *e, EVP_PKEY_ASN1_METHOD **ameth, const int **nids, int nid) {
    if (!ameth) {
        *nids = meth_nids;
        return sizeof(meth_nids) / sizeof(meth_nids[0]) - 1;
    }

    *ameth = EVP_PKEY_asn1_new(nid, 0, "kyber512", "NIST PQC Kyber512");
    EVP_PKEY_asn1_set_public(*ameth, pub_decode, 
                                    pub_encode, 
                                    pub_cmp,
                                    pub_print,
                                    pkey_size,
                                    pkey_bits);
    EVP_PKEY_asn1_set_private(*ameth, priv_decode,
                                      priv_encode,
                                      priv_print);
    EVP_PKEY_asn1_set_security_bits(*ameth, pkey_security_bits);
    EVP_PKEY_asn1_set_free(*ameth, pkey_free);
    EVP_PKEY_asn1_set_ctrl(*ameth, pkey_asn_ctrl);

    // cmp_parameters
    // item_verify
    // item_sign
    // sig_info_set_
    return 1;
}


static int bind(ENGINE *e, const char *id) {
    static int loaded = 0;
    if (loaded) {
        fprintf(stderr, "NIST PQCrypto engine already loaded\n");
        return 0;
    }


  if (!ENGINE_set_id(e, "nistpqc") ||
      !ENGINE_set_name(e, "NIST PQCrypto") || 
      !ENGINE_set_pkey_meths(e, pkey_meths) ||
      !ENGINE_set_pkey_asn1_meths(e, pkey_asn1_meths)) {
    printf("Unexpected engine setup fail\n");
    return 0;
  }


  loaded = 1;

/*
    || !ENGINE_set_flags(e, ENGINE_FLAGS_NO_REGISTER_ALL)
    || !ENGINE_set_finish_function(e, capi_finish)
    || !ENGINE_set_destroy_function(e, capi_destroy)
    || !ENGINE_set_load_privkey_function(e, capi_load_privkey)
    || !ENGINE_set_load_ssl_client_cert_function(e, capi_load_ssl_client_cert)
    || !ENGINE_set_cmd_defns(e, capi_cmd_defns)
    || !ENGINE_set_ctrl_function(e, capi_ctrl))

    */
    return 1;
}

IMPLEMENT_DYNAMIC_BIND_FN(bind)

OPENSSL_EXPORT unsigned long v_check(unsigned long v) {
    return 0x00030000+1;//OSSL_DYNAMIC_VERSION + 1;
}
