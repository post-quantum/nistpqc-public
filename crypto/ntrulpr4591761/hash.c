#include <stddef.h>
#include <openssl/sha.h>

int crypto_hash_sha512_openssl(unsigned char *out,const unsigned char *in,unsigned long long inlen)
{
  SHA512(in,inlen,out);
  return 0;
}
