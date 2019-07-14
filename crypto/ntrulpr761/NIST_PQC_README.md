### Version

The version of NTRU LPrime 4591^761 used here is from the [reference implementation](https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/NTRU_Prime.zip) submitted to NIST Post-Quantum Cryptography Standardization.

### Changes

- Removed `nist/rng.c` and `nist/kat_kem.c`
- Replaced `Makefile` with our own version
- Copied `crypto_int8.h` from SUPERCOP source-code (version 20171218)
- Copied `crypto_uint8.h` from SUPERCOP source-code (version 20171218)
- Copied `crypto_int16.h` from SUPERCOP source-code (version 20171218)
- Copied `crypto_uint16.h` from SUPERCOP source-code (version 20171218)
- Copied `crypto_int32.h` from SUPERCOP source-code (version 20171218)
- Copied `crypto_uint32.h` from SUPERCOP source-code (version 20171218)
- Copied `crypto_stream_sha512.h` from SUPERCOP source-code (version 20171218)
- Copied `crypto_stream_aes256ctr.h` from SUPERCOP source-code (version 20171218)
- Copied `hash.c` from SUPERCOP source-code (version 20171218), OpenSSL version of SHA512
- Copied `stream.c` from SUPERCOP source-code (version 20171218), OpenSSL version of AES256CTR
- Updated `#include` header in `randombytes.h` to reference to `rng.h` instead of `nist/rng.h`
- Added `CRYPTO_ALGNAME` definition to `api.h`

