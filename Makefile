
test: test.o
	ld -o test test.o

test.o: test.s
	as -o test.o test.s

modify: test.c
	gcc -Wall test.c

clean:
	rm a.out test test.o

