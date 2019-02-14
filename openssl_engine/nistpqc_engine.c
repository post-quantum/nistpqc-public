#include "../nistpqc_api.h"
//#include "nistpqc_api_internal.h"
#include <stdio.h>
#include <string.h>

#include <openssl/engine.h>

static const char* engine_id = "nistpqc";
static const char* engine_name = "NIST PQCrypto";

// Insist on OpenSSL 1.1.0 or later
#if OPENSSL_VERSION_NUMBER < 0x10100000
#error Requires OpenSSL 1.1 or later 
#endif

//static int nistpqc_ctrl(ENGINE* e, int cmd, long, void *, void(*f)(void))
#define NID_nistpqc_kyber512 2004




static int nistpqc_pkey_init(EVP_PKEY_CTX *ctx, nistpqc_cipher_t cipher) {
    nistpqc_t* nistpqc = OPENSSL_malloc(sizeof(nistpqc_t));
    if (!nistpqc) {
        return 0;
    }
    int rv = nistpqc_init(nistpqc, cipher);

    EVP_PKEY_CTX_set_data(ctx, nistpqc);
    return rv;
}
static int kyber512_pkey_init(EVP_PKEY_CTX *ctx) {
    return nistpqc_pkey_init(ctx, NISTPQC_CIPHER_KYBER512);
}



static const int meth_nids[] = {NID_nistpqc_kyber512, 0};


typedef struct {
  nistpqc_t* nistpqc;
  int nid;
  int is_server;
  size_t pk_size;
  size_t sk_size;
  uint8_t *pubkey;
  uint8_t *privkey;
} NISTPQC_KEYINFO;

static int pkey_keygen(EVP_PKEY_CTX *ctx, EVP_PKEY *pkey) {
    printf("keygen\n");
    
    if (pkey == NULL || ctx == NULL) {
        return 0;
    }
    nistpqc_t* nistpqc = EVP_PKEY_CTX_get_data(ctx);
    if (nistpqc == NULL) {
        return 0;
    }

    NISTPQC_KEYINFO* keyinfo = OPENSSL_zalloc(sizeof(NISTPQC_KEYINFO));
    keyinfo->nistpqc = nistpqc;

    EVP_PKEY* peerKey = EVP_PKEY_CTX_get0_pkey(ctx);

    // If the peer key doesn't exist, this is the client side and we must generate a keypair
    if (!peerKey) {
        printf("keygen client - generating keypair");
        keyinfo->is_server = 0;

        // Allocations to hold the keypair
        keyinfo->pk_size = nistpqc->public_key_size();
        keyinfo->sk_size = nistpqc->private_key_size();
        keyinfo->pubkey = OPENSSL_malloc(keyinfo->pk_size);
        keyinfo->privkey = OPENSSL_secure_malloc(keyinfo->sk_size);

        // Generate the keypair
        if (0 != nistpqc->keypair(keyinfo->pubkey, keyinfo->privkey)) {
            // todo: errors
            return 0;
        }

    } else {
        keyinfo->is_server = 1;

        // Server-side KEM. First we generate the shared secret and pretend its our private key
        size_t shared_secret_size = nistpqc->shared_secret_size();
        unsigned char* shared_secret = OPENSSL_secure_malloc(shared_secret_size);
        RAND_bytes(shared_secret, shared_secret_size);
        keyinfo->sk_size = shared_secret_size;
        keyinfo->privkey = shared_secret;

        // Encapsulate the shared secret using the peer public key and that's our
        // "public key" that goes into the keyshare extension sent back to the client.
        NISTPQC_KEYINFO* peerKeyinfo = (NISTPQC_KEYINFO*)EVP_PKEY_get0(peerKey);
        keyinfo->pk_size = nistpqc->ciphertext_size();
        keyinfo->pubkey = OPENSSL_malloc(keyinfo->pk_size);
        printf("Calling enc() ");
        if (0 != nistpqc->enc(keyinfo->pubkey, shared_secret, peerKeyinfo->pubkey)) {
            // todo: errors
            return 0;
        }
    }
    
    // Success!
    return EVP_PKEY_assign(pkey, NID_nistpqc_kyber512 /* todo: make nistpqc return nid */, keyinfo); 
}

static int pkey_ctrl(EVP_PKEY_CTX *ctx, int type, int p1, void *p2) {
    /* Only need to handle peer key for derivation */
    if (type == EVP_PKEY_CTRL_PEER_KEY)
        return 1;
    return -2;
}
static int pkey_derive(EVP_PKEY_CTX *ctx, unsigned char *key,size_t *keylen) {
    printf("derive\n");
    EVP_PKEY* pkey = EVP_PKEY_CTX_get0_pkey(ctx);
    NISTPQC_KEYINFO* keyinfo = (NISTPQC_KEYINFO*)EVP_PKEY_get0(pkey);

    // Server: just copy the shared secret out
    if (keyinfo->is_server) {
        if (!key) {
            *keylen = keyinfo->sk_size;
        } else {
            memcpy(key, keyinfo->privkey, keyinfo->sk_size);
        }
        return keyinfo->sk_size;
    }

    // Client: decapsulate the server's "public key" to get the shared secret
    else {
        if (!key) {
            *keylen = keyinfo->nistpqc->shared_secret_size();
        } else {
            EVP_PKEY* peerKey = EVP_PKEY_CTX_get0_peerkey(ctx);
            NISTPQC_KEYINFO* peerKeyinfo = (NISTPQC_KEYINFO*)EVP_PKEY_get0(peerKey);
            keyinfo->nistpqc->dec(key, peerKeyinfo->pubkey, keyinfo->privkey);
        }
        return *keylen;
    }
    return 0;
}


static int nistpqc_pkey_meths(ENGINE *e, EVP_PKEY_METHOD **pmeth, const int **nids, int nid) {
    if (!pmeth) {
        *nids = meth_nids;
        return sizeof(meth_nids) / sizeof(meth_nids[0]) - 1;
    }

    if (nid == NID_nistpqc_kyber512) {
        *pmeth = EVP_PKEY_meth_new(nid, EVP_PKEY_FLAG_SIGCTX_CUSTOM);
        EVP_PKEY_meth_set_keygen(*pmeth, kyber512_pkey_init, pkey_keygen);
        EVP_PKEY_meth_set_derive(*pmeth, NULL, pkey_derive);
        EVP_PKEY_meth_set_ctrl(*pmeth, pkey_ctrl, NULL);
        return 1;
    }

    return 0;
}

static int nistpqc_pkey_ctrl(EVP_PKEY *pkey, int op, long arg1, void *arg2) {
    if (op == ASN1_PKEY_CTRL_GET1_TLS_ENCPT) {
        unsigned char **ppt = arg2;
        // Just copy public key 
       NISTPQC_KEYINFO* keyinfo = EVP_PKEY_get0(pkey);
       *ppt = OPENSSL_memdup(keyinfo->pubkey, keyinfo->pk_size);
        return (*ppt) ? keyinfo->pk_size : 0;
    }
    if (op == ASN1_PKEY_CTRL_SET1_TLS_ENCPT) {
        NISTPQC_KEYINFO* keyinfo = EVP_PKEY_get0(pkey);
        if (!keyinfo) {
            keyinfo = OPENSSL_zalloc(sizeof(NISTPQC_KEYINFO));
            EVP_PKEY_assign(pkey, NID_nistpqc_kyber512, keyinfo);
        }
        keyinfo->pk_size = arg1;
        keyinfo->pubkey = OPENSSL_memdup(arg2, arg1);
        return arg1;
    }
    printf("todo: nistpqc_pkey_ctrl %d\n", op);
    return 0;
}
static int nistpqc_pub_decode(EVP_PKEY *pk, X509_PUBKEY *pub) {
    printf("todo: nistpqc_pub_decode\n");
    return 0;
}
static int nistpqc_pub_encode(X509_PUBKEY *pub, const EVP_PKEY *pk) {
    printf("todo: nistpqc_pub_encode\n");
    return 0;
}
static int nistpqc_pub_cmp(const EVP_PKEY *a, const EVP_PKEY *b) {
    printf("todo: nistpqc_pub_cmp\n");
    return 0;
}
static int nistpqc_pub_print(BIO *out, const EVP_PKEY *pkey, int indent, ASN1_PCTX *pctx) {
    printf("todo: nistpqc_pub_print\n");
    return 0;
}
static int nistpqc_pkey_size(const EVP_PKEY *pk) {
    printf("todo: nistpqc_pkey_size\n");
    return 0;
}
static int nistpqc_pkey_bits(const EVP_PKEY *pk) {
    printf("todo: nistpqc_pkey_bits\n");
    return 0;
}
static int nistpqc_priv_decode(EVP_PKEY *pk, const PKCS8_PRIV_KEY_INFO *p8inf) {
    printf("todo: nistpqc_priv_decode\n");
    return 0;
}
static int nistpqc_priv_encode(PKCS8_PRIV_KEY_INFO *p8, const EVP_PKEY *pk) {
    printf("todo: nistpqc_priv_encode\n");
    return 0;
}
static int nistpqc_priv_print(BIO *out, const EVP_PKEY *pkey, int indent, ASN1_PCTX *pctx) {
    printf("todo: nistpqc_priv_print\n");
    return 0;
}
static int nistpqc_pkey_security_bits(const EVP_PKEY *pk) {
    printf("todo: nistpqc_pkey_security_bits\n");
    return 0;
}
static void nistpqc_pkey_free(EVP_PKEY *pk) {
     NISTPQC_KEYINFO* keyinfo =  EVP_PKEY_get0(pk);
     if (keyinfo) {
         if (keyinfo->privkey) {
             OPENSSL_secure_free(keyinfo->privkey);
         }
         if (keyinfo->pubkey) {
             OPENSSL_free(keyinfo->pubkey);
         }
        OPENSSL_free(keyinfo);
     }
}

static int nistpqc_pkey_asn1_meths(ENGINE *e, EVP_PKEY_ASN1_METHOD **ameth, const int **nids, int nid) {
    if (!ameth) {
        *nids = meth_nids;
        return sizeof(meth_nids) / sizeof(meth_nids[0]) - 1;
    }

    if (nid == NID_nistpqc_kyber512) {
        *ameth = EVP_PKEY_asn1_new(nid, 0, "kyber512", "NIST PQC Kyber512");
        EVP_PKEY_asn1_set_public(*ameth, nistpqc_pub_decode, 
                                         nistpqc_pub_encode, 
                                         nistpqc_pub_cmp,
                                         nistpqc_pub_print,
                                         nistpqc_pkey_size,
                                         nistpqc_pkey_bits);
        EVP_PKEY_asn1_set_private(*ameth, nistpqc_priv_decode,
                                          nistpqc_priv_encode,
                                          nistpqc_priv_print);
        EVP_PKEY_asn1_set_security_bits(*ameth, nistpqc_pkey_security_bits);

        //nistpqc_cmp_parameters
        EVP_PKEY_asn1_set_free(*ameth, nistpqc_pkey_free);

        EVP_PKEY_asn1_set_ctrl(*ameth, nistpqc_pkey_ctrl);

        // nistpqc_item_verify
        // nistpqc_item_sign
        // nistpqc_sig_info_set_
        return 1;
    }

    return 0;
}


static int bind(ENGINE *e, const char *id) {
    static int loaded = 0;
    if (loaded) {
        fprintf(stderr, "NIST PQCrypto engine already loaded\n");
        return 0;
    }


  if (!ENGINE_set_id(e, engine_id) ||
      !ENGINE_set_name(e, engine_name) || 
      !ENGINE_set_pkey_meths(e, nistpqc_pkey_meths) ||
      !ENGINE_set_pkey_asn1_meths(e, nistpqc_pkey_asn1_meths)) {
      //!ENGINE_set_init_function(e, capi_init) {
    printf("Unexpected engine setup fail\n");
    return 0;
  }


  loaded = 1;

/*
    || !ENGINE_set_flags(e, ENGINE_FLAGS_NO_REGISTER_ALL)
    || !ENGINE_set_finish_function(e, capi_finish)
    || !ENGINE_set_destroy_function(e, capi_destroy)
    || !ENGINE_set_load_privkey_function(e, capi_load_privkey)
    || !ENGINE_set_load_ssl_client_cert_function(e,
                                                 capi_load_ssl_client_cert)
    || !ENGINE_set_cmd_defns(e, capi_cmd_defns)
    || !ENGINE_set_ctrl_function(e, capi_ctrl))

    */
    return 1;
}

IMPLEMENT_DYNAMIC_BIND_FN(bind)

OPENSSL_EXPORT unsigned long v_check(unsigned long v) {
    return 0x00030000+1;//OSSL_DYNAMIC_VERSION + 1;
}
