#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>

#include "args.h"

#define SIZE (array_max * sizeof(unsigned long long))
#define ull unsigned long long

ull isqrt(ull i)
{
    ull l = 0;
    ull r = i + 1;
    while (l != r - 1)
    {
        ull m = (l + r) / 2;
        if (m * m <= i)
            l = m;
        else
            r = m;
    }
    return l;
}

void show_help(char* argv0)
{
    fprintf(stderr,
            "Usage: %s range [args]\n"
            "  Check primes from 0 to range (Maximum %llu)\n\n"

            "Args:"
            "    -p, --print        Print primes found\n",
            argv0,
            ULLONG_MAX);
}

int main(int argc, char** argv)
{
    if(argc <= 1)
    {
        show_help(argv[0]);
        return EXIT_FAILURE;
    }

    _Args args;
    init_args(&args);
    parse_args(argc, argv, &args);

    /* check primes up to primes_stop */
    ull primes_stop = strtoull(argv[1], NULL, 10);
    if(primes_stop == 0)
    {
        show_help(argv[0]);
        return EXIT_FAILURE;
    }

    ull array_max = 1;
    ull old_size = SIZE;

    ull *primes = mmap64(NULL, SIZE, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE | MAP_NORESERVE, -1, sysconf(_SC_PAGESIZE));
    if(primes == MAP_FAILED)
    {
        perror("mmap");
        return EXIT_FAILURE;
    }
    primes[0] = 2;

    clock_t begin_primes = clock();
    /* number_curr will never be even, so it is useless to check if it is, by dividing it with 2 (primes[0]) */
    for(ull number_curr = 3; number_curr <= primes_stop; number_curr += 2)
    {
        _Bool is_prime = 1;

        for(ull primes_indx = 0; (primes[primes_indx] <= isqrt(number_curr)) && (primes_indx < array_max); ++primes_indx)
        {
            if(number_curr % primes[primes_indx] == 0)
            {
                is_prime = 0;
                break;
            }
        }

        if(is_prime)
        {
            old_size = SIZE;
            array_max += 1;
            primes = mremap(primes, old_size, SIZE, MREMAP_MAYMOVE);
            if(primes == MAP_FAILED)
            {
                perror("mremap");
                return EXIT_FAILURE;
            }

            primes[array_max - 1] = number_curr;
        }
    }
    clock_t end_primes = clock();

    clock_t begin_print = clock();
    if(args.printing)
    {
        for(ull primes_indx = 0; primes_indx < array_max; ++primes_indx)
            /* TODO: optimise printing */
            printf("%llu\n", primes[primes_indx]);
    }
    clock_t end_print = clock();

    printf("Counted %llu primes\nCalculation took: %0.fms\n",
        array_max,
        ((double)(end_primes - begin_primes) / CLOCKS_PER_SEC) * (double)1000);

    if(args.printing)
        printf("Printing took %0.fms\n",
            ((double)(end_print  - begin_print ) / CLOCKS_PER_SEC) * (double)1000);

    if(munmap(primes, SIZE) == -1)
    {
        perror("munmap");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

