/**
 * rng.c
 *
 * OpenSSL wrapper of randombytes method
 **/

#include "rng.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define RANDOM_DEVICE	"/dev/urandom"
#define RANDOM_MAX_LEN	1048576

int randombytes(unsigned char *x, unsigned long long xlen)
{
	int size = 0;
	unsigned char *x_ptr = x;
	static int fd = -1;
	
	if (fd == -1)
	{
		while (1)
		{
			if (-1 != (fd = open(RANDOM_DEVICE, O_RDONLY)))
				break;
			sleep(1);
		}
	}

	while (xlen > 0)
	{
		if (xlen < RANDOM_MAX_LEN)
		{
			size = xlen;
		}
		else
		{
			size = RANDOM_MAX_LEN;
		}

		size = read(fd, x_ptr, size);
		if (size < 1)
		{
			sleep(1);
			continue;
		}

		x_ptr += size;
		xlen -= size;
	}

	return 0;
}
