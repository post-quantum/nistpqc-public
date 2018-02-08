/**
 * NIST PQC test suites
 *
 * Included ciphers:
 *   - NewHope 512 CCA
 *   - Kyber 512
 *   - NTRU LPrime 4591^761
 *   - NTRU KEM 443
 *   - SIKE p503
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "nistpqc_api.h"

#define FIRST_PQC_CIPHER	NISTPQC_NEWHOPE512CCA
#define LAST_PQC_CIPHER		NISTPQC_SIKEP503
#define TEST_RUN		    100
#define BENCHMARK_RUN       100
#define PASSED              1
#define FAILED              0

uint64_t cpucycles(void)
{
	uint64_t a;

	asm volatile (".byte 15;.byte 49;shlq $32,%%rdx;orq %%rdx,%%rax" : "=a" (a) :: "%rdx");

	return a;
}

int nistpqc_kem_test(nistpqc_cipher_t cipher)
{
	int i, status = PASSED;
	uint8_t *pk, *sk;
	uint8_t *ct, *ss, *_ss;
	nistpqc_t nistpqc;

	if (!nistpqc_init(&nistpqc, cipher)) 
	{
		fprintf(stderr, "Unable to initialise nistpqc object\n\n");
		return FAILED;
	}

	pk  = (uint8_t *)calloc(nistpqc.public_key_size(), sizeof(uint8_t));
	sk  = (uint8_t *)calloc(nistpqc.private_key_size(), sizeof(uint8_t));
	ct  = (uint8_t *)calloc(nistpqc.ciphertext_size(), sizeof(uint8_t));
	ss  = (uint8_t *)calloc(nistpqc.shared_secret_size(), sizeof(uint8_t));
	_ss = (uint8_t *)calloc(nistpqc.shared_secret_size(), sizeof(uint8_t));

    fprintf(stdout, "Testing KEM cipher: %s\n", nistpqc.algorithm_name());
	fflush(stdout);
	for (i=0; i<TEST_RUN; i++)
	{
		memset(pk,  0, nistpqc.public_key_size());
		memset(sk,  0, nistpqc.private_key_size());
		memset(ct,  0, nistpqc.ciphertext_size());
		memset(ss,  0, nistpqc.shared_secret_size());
		memset(_ss, 0, nistpqc.shared_secret_size());

		if (nistpqc.keypair(pk, sk)) 
		{
			fprintf(stderr, "Failed to generate a key-pair\n"); fflush(stderr);
			status = FAILED;
			break;
		}
		if (nistpqc.enc(ct, ss, pk)) 
		{
			fprintf(stderr, "Failed to do encapsulation\n"); fflush(stderr);
			status = FAILED;
			break;
		}
		if (nistpqc.dec(_ss, ct, sk)) 
		{
			fprintf(stderr, "Failed to do decapsulation\n"); fflush(stderr);
			status = FAILED;
			break;
		}

		if (memcmp(ss, _ss, nistpqc.shared_secret_size()))
		{
			status = FAILED;
			fprintf(stderr, "Shared secret mismatch!\n"); fflush(stderr);
			break;
		}
	}

	fprintf(stdout, "Testing completed, status: %s\n", status == PASSED ? "PASSED" : "FAILED");

	free(pk);
	free(sk);
	free(ct);
	free(ss);
	free(_ss);

	return status;
}

int nistpqc_kem_benchmark(nistpqc_cipher_t cipher)
{
	int i;
	uint8_t *pk, *sk;
	uint8_t *ct, *ss;
	nistpqc_t nistpqc;
	uint64_t start, stop, cycles;

	if (!nistpqc_init(&nistpqc, cipher)) 
	{
		fprintf(stderr, "Unable to initialise nistpqc object\n\n");
		return FAILED;
	}

	pk  = (uint8_t *)calloc(nistpqc.public_key_size(), sizeof(uint8_t));
	sk  = (uint8_t *)calloc(nistpqc.private_key_size(), sizeof(uint8_t));
	ct  = (uint8_t *)calloc(nistpqc.ciphertext_size(), sizeof(uint8_t));
	ss  = (uint8_t *)calloc(nistpqc.shared_secret_size(), sizeof(uint8_t));

    fprintf(stdout, "Benchmarking KEM cipher: %s\n", nistpqc.algorithm_name());
	fflush(stdout);

	cycles = 0ULL;
	fprintf(stdout, "  Key-pair generation takes on average "); fflush(stdout);
	for (i=0; i<BENCHMARK_RUN; i++)
	{
		start = cpucycles();
		nistpqc.keypair(pk, sk);
		stop  = cpucycles();
		cycles += (stop - start);
	}
	fprintf(stdout, "%lu cycles\n", cycles/BENCHMARK_RUN);
	
	cycles = 0ULL;
	fprintf(stdout, "  Encapsulation takes on average "); fflush(stdout);
	for (i=0; i<BENCHMARK_RUN; i++)
	{
		start = cpucycles();
		nistpqc.enc(ct, ss, pk);
		stop  = cpucycles();
		cycles += (stop - start);
	}
	fprintf(stdout, "%lu cycles\n", cycles/BENCHMARK_RUN);
	
	cycles = 0ULL;
	fprintf(stdout, "  Decapsulation takes on average "); fflush(stdout);
	for (i=0; i<BENCHMARK_RUN; i++)
	{
		start = cpucycles();
		nistpqc.dec(ss, ct, sk);
		stop  = cpucycles();
		cycles += (stop - start);
	}
	fprintf(stdout, "%lu cycles\n", cycles/BENCHMARK_RUN);

	return PASSED;
}

int main()
{
	int status = PASSED;
	nistpqc_cipher_t cipher;

	for (cipher=FIRST_PQC_CIPHER; cipher<=LAST_PQC_CIPHER && status; cipher++)
	{
		status = nistpqc_kem_test(cipher);
		status = nistpqc_kem_benchmark(cipher);
		fprintf(stdout, "-------------------\n");
	}
	
	return status;
}
