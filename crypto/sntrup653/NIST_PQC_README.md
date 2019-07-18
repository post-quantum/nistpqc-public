### Version

The version of Streamlined NTRU Prime 653 used here is from the [reference implementation](https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/NTRU-Prime-Round2.zip) submitted to NIST Post-Quantum Cryptography Standardization.

### Changes

- Removed `KATNUM`, `build`, `run`, `Makefile`, `nist/rng.c`, and `nist/kat_kem.c`
- Added `CRYPTO_ALGNAME` definition to `api.h`
- Added `pk_from_sk.c`
