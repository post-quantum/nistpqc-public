# **NIST PQC Library**

This library implements a number of post-quantum key-establishment algorithms that have been submitted to the second round of NIST PQC standardization process. It has been used to provide post-quantum key-exchange for:
* hybrid IKEv2 as per [draft-tjhai-ipsecme-hybrid-qske-ikev2-00](https://tools.ietf.org/html/draft-tjhai-ipsecme-hybrid-qske-ikev2-00) IETF draft implemented using [strongSwan](https://github.com/post-quantum/strongswan/tree/qske); and 
* hybrid key-exchange for TLS 1.3 using secondary key shares implemented using [OpenSSL](https://github.com/post-quantum/openssl/tree/nistpqc).

The following key-establishment algorithms have been integrated into this library:
```
+----------------------------+---------------+-----------------+-----------------+
|          Algorithm         | NIST security | Public-key size | Ciphertext size |
|                            |    category   |     (bytes)     |     (bytes)     |
+----------------------------+---------------+-----------------+-----------------+
|                             Lattice-based algorithms                           |
+----------------------------+---------------+-----------------+-----------------+
| NewHope 512 CCA            |        1      |        928      |       1120      |
| NewHope 1024 CCA           |        5      |       1824      |       2208      |
| Kyber 512                  |        1      |        800      |        736      |
| Kyber 768                  |        3      |       1184      |       1088      |
| Kyber 1024                 |        5      |       1568      |       1568      |
| NTRU LPRime 653            |        2      |        897      |       1025      |
| NTRU LPRime 761            |        3      |       1039      |       1167      |
| NTRU LPRime 857            |        4      |       1184      |       1312      |
| Streamlined NTRU Prime 653 |        2      |        994      |        897      |
| Streamlined NTRU Prime 761 |        3      |       1158      |       1039      |
| Streamlined NTRU Prime 857 |        4      |       1322      |       1184      |
| NTRU HRSS 701              |        3      |       1138      |       1138      |
| NTRU HPS 2048 509          |        1      |        699      |        699      |
| NTRU HPS 2048 677          |        3      |        930      |        930      |
| NTRU HPS 4096 821          |        5      |       1230      |       1230      |
| LAC 128                    |        1      |        544      |        712      |
| LAC 192                    |        3      |       1056      |       1188      |
| LAC 256                    |        5      |       1056      |        712      |
| LightSaber                 |        1      |        672      |        736      |
| Saber                      |        3      |        992      |       1088      |
| FireSaber                  |        5      |       1312      |       1472      |
| Frodo KEM 640              |        1      |       9616      |       9720      |
| Frodo KEM 976              |        3      |      15632      |      15744      |
| Frodo KEM 1344             |        5      |      21520      |      21632      |
+----------------------------+---------------+-----------------+-----------------+
|                              Code-based algorithms                             |
+----------------------------+---------------+-----------------+-----------------+
| LEDAKEM 128                |        1      |       2080      |       1040      |
| LEDAKEM 192                |        3      |       4032      |       2016      |
| LEDAKEM 256                |        5      |       4616      |       4616      |
+----------------------------+---------------+-----------------+-----------------+
|                            Isogeny-based algorithms                            |
+----------------------------+---------------+-----------------+-----------------+
| SIKEp434                   |        1      |        330      |        346      |
| SIKEp503                   |        1      |        378      |        402      |
| SIKEp610                   |        3      |        462      |        486      |
| SIKEp751                   |        5      |        564      |        596      |
+----------------------------+---------------+-----------------+-----------------+
```

Adding a new cipher is relatively easy and it generally involves the following steps:

1. Copy the source code of a candidate PQC algorithm to NIST PQC source directory, i.e. `nistpqc/crypto` directory. The candidate algorithm shall be given a name that does not contain a space, hyphen, or underscore symbol. This name will be used as a prefix in the context of NIST PQC library and also as the algorithm name when integrated with OpenSSL. If it is not already done, rename the directory containing the source code with this name.

    There are a number of candidate algorithms that share the same code-base for all proposed parameters. In NIST PQC library, because each specific parameter set requires its own source code directory, it is inevitable that there will be code duplication. Nonetheless this is not an issue as each parameter set will have its own unique algorithm name as the prefix.

2. Removed unused files such as `Makefile`, `rng.c`, `rng.h` and any KAT generation files such as `PQCgenKAT.c`. Record any changes to the original source code in `NIST_PQC_README.md` file that lives in the same directory as the algorithm source code. Note that we can remove `rng.c` file because NIST PQC library provides a fallback implementation. However, it may not be possible to remove this file for all candidates and LEDAKEM is such an example. In the case of LEDAKEM, we exclude some random number generation functions from being compiled.

3. The `Makefile` in NIST PQC library will enumerate and compile all the C source files of a candidate algorithm. However, this will not work if the algorithm implementers include a C source file from another source file. In this case, we will need to inspect the content of the original `Makefile` and enumerate the necessary source files that need to be compiled into a file named `SOURCES`.

    If the compilation requires any compiler specific flags, we also need to add these to a variable named `xyzcipher_DEFINES` where `xyzcipher` is the name/prefix mentioned above.

4. Edit `nistpqc_api.h` to add a new enumeration for the new algorithm.

5. Implement `crypto_public_key_from_private_key` method which, as the name implies, generates a public-key from a given private-key. This step is not mandatory as this function API is not part of NIST requirements. If this method is not implemented, then a default non-operational version will be used instead.

6. Export the required methods in `nistpqc_api.c` with `DECLARE_CIPHER_EXPORTS` macro, and initialise the new algorithm by assigning function pointers to the exported methods with `INIT_CIPHER_INTERFACE` macro.

7. Create a `symbols.map` file, which is a text file that controls the renaming of functions exported by a NIST PQC algorithm. Each line in `symbols.map` file contains two identifiers: the first is the function name as it is compiled, the second is the exported function name, i.e. the one with the algorithm name prefixed in order to avoid collision with corresponding functions of other algorithms.

8. Edit the file `test/nistpqc_test.c` to include the added PQC algorithm. This step is not necessary if the algorithm is not added as the last algorithm.

## **Prerequisites**

In order to compile the library, you will need the development headers of OpenSSL. If you are linking it to produce an executable, you will need to link it against `libcrypto` of OpenSSL.

## **How to Build**

In order to build and install the library, just run `make` and `make install` respectively. By the default, the shared and static library will be installed in `/usr/local` prefix. If this does not suit you, edit the `Makefile`.

## **Tests**

Tests are available by executing `make test` command and it will produce a binary in `build/native/nistpqc_test`. This binary will run 100 iterations of key-generation, encapsulation and decapsulation for each included post-quantum algorithm where in each iteration, it creates a random key-pair, generates a random shared-secret, encapsulates the shared-secret, decapsulates the shared-secret and lastly verifies the correctness of the decapsulated output.