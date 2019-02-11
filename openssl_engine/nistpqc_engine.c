#include "../nistpqc_api.h"
//#include "nistpqc_api_internal.h"
#include <stdio.h>
#include <string.h>

#include <openssl/engine.h>

static const char *engine_id = "nistpqc";
static const char *engine_name = "NIST PQCrypto";



static int bind(ENGINE *e, const char *id) {
  static int loaded = 0;
  int ret = 0;

  if (id && strcmp(id, engine_id)) {
    goto end;
  }

  if (!ENGINE_set_id(e, engine_id)) {
    fprintf(stderr, "ENGINE_set_id failed\n");
    goto end;
  }
  if (!ENGINE_set_name(e, engine_name)) {
    printf("ENGINE_set_name failed\n");
    goto end;
  }

  if (loaded) {
    fprintf(stderr, "NIST PQCrypto engine already loaded\n");
    goto end;
  }

  loaded = 1;


    ret = 1;
 end:
    return ret;
}

IMPLEMENT_DYNAMIC_BIND_FN(bind)

OPENSSL_EXPORT unsigned long v_check(unsigned long v) {
    return 0x00030000+1;//OSSL_DYNAMIC_VERSION + 1;
}
