test_barr: barr.o test_barr.o
	gcc -ansi -pedantic -Wall -o test_barr test_barr.o barr.o

test_barr.o: test_barr.c barr.h
	gcc -ansi -pedantic -Wall -c test_barr.c

barr.o: barr.c barr.h
	gcc -ansi -pedantic -Wall -c barr.c

clean:
	rm *.o test_barr
