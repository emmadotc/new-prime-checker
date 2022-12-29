FILES = main.c args.c

default:
	gcc -std=c99 -Os -Wall -Wextra -Wpedantic -Wconversion -Wvla -march=native $(FILES) -o primes

clean:
	rm ./primes*

bear:
	make clean ; bear -- make
