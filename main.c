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

#include "common.h"

#include "args.h"
#include "methods/methods.h"

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
    if(!parse_args(argc, argv, &args))
    {
        show_help(argv[0]);
        return EXIT_FAILURE;
    }

    /* check primes up to primes_stop */
    ull primes_stop = strtoull(argv[1], NULL, 10);
    if(primes_stop == 0)
    {
        show_help(argv[0]);
        return EXIT_FAILURE;
    }

    ull array_max = 0;
    ull *primes = NULL;

    clock_t begin_primes = clock();
    if(!simple_for(primes_stop, primes, &array_max))
    {
        return EXIT_FAILURE;
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

