### Version

The version of LEDAkem (NIST category 3, n_0=3) used here is from the [optimized implementation](https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-2/submissions/LEDAcrypt-Round2.zip) submitted to NIST Post-Quantum Cryptography Standardization.

### Changes
- Commented out the following methods from `rng.c`: 
  o `void initialize_pseudo_random_generator_seed(int, char*[])`
  o `void randombytes_init(unsigned char*, unsigned char*, int)`
  o `int randombytes(unsigned char*, unsigned long long)`
  They are not needed, instead we will use one using OpenSSL wrapper, see
  `../../common/rng.c`
- Flattened directory structure, no `bin`, `include` or `library` directories
- Removed `Makefile`
- Used `sha3.c|h` from Round 1 to avoid external lib dependency
- Replace LEDA with LEDAKEM192 in `api.h`
