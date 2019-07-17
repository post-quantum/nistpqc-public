### Version

The version of NTRU LPrime 4591^761 used here is from the [reference implementation](https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/NTRU_Prime.zip) submitted to NIST Post-Quantum Cryptography Standardization.

### Changes

- Removed `nist/rng.c` and `nist/kat_kem.c`
- Replaced `Makefile` with our own version
- Updated `#include` header in `randombytes.h` to reference to `rng.h` instead of `nist/rng.h`
- Added `CRYPTO_ALGNAME` definition to `api.h`

