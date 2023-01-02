CC    = gcc

FILES = main.c args.c common.c methods/simple_for.c
WARNS = -Wall -Wextra -Wpedantic -Wconversion -Wvla
OPTIM = -Os -march=native

default:
	$(CC) -std=c99 $(OPTIM) $(WARNS) $(FILES) -o primes

clean:
	rm ./primes*

bear:
	make clean ; bear -- make
