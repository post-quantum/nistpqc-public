### Version

The version of NTRU-KEM-443 used here is from the [reference implementation](https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/NTRUEncrypt.zip) submitted to NIST Post-Quantum Cryptography Standardization.

### Changes

- Removed `PQCgenKAT_kem.c`
- Removed `makefile`
- Moved `crypto_hash_sha512.[h,c]` from `../common` directory
- Moved `crypto_stream.c from` `../common` directory
- Moved `crypto_stream_salsa20.h` from `../common` directory
- Moved `fastrandombytes.[h,c]` from `../common` directory
- Moved `shred.[h,c]` from `../common` directory
- Updated the `#include` header in `kem.c` to not reference from `../common` directory
- Updated the `#include` header in `NTRUEncrypt.c` to not reference from `../common` directory
- Updated the `#include` header in `poly.c` to not reference from `../common` directory


