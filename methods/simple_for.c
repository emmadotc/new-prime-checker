#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#include "methods.h"
#include "../common.h"

clock_t simple_for(ull primes_stop, ull* primes, ull* array_max)
{
    primes[0] = 2;
    ull old_size = SIZEP;

    clock_t begin = clock();
    /* number_curr will never be even, so it is useless to check if it is, by dividing it with 2 (primes[0]) */
    for(ull number_curr = 3; number_curr <= primes_stop; number_curr += 2)
    {
        _Bool is_prime = 1;

        for(ull primes_indx = 0; (primes[primes_indx] <= isqrt(number_curr)) && (primes_indx < *array_max); ++primes_indx)
        {
            if(number_curr % primes[primes_indx] == 0)
            {
                is_prime = 0;
                break;
            }
        }

        if(is_prime)
        {
            old_size = SIZEP;
            *array_max += 1;
            primes = mremap(primes, old_size, SIZEP, MREMAP_MAYMOVE);
            if(primes == MAP_FAILED)
            {
                perror("mremap");
                return -2;
            }

            primes[*array_max - 1] = number_curr;
        }
    }

    return begin;
}

