default:
	gcc -std=c99 -Os -Wall -Wextra -Wpedantic -Wconversion -Wvla -march=native main.c -o np

clean:
	rm ./np*

bear:
	make clean ; bear -- make
