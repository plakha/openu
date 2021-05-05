prog: test_list.o list.o
	gcc -ansi -pedantic -Wall -o test_list test_list.o list.o
		
test_list.o: list.h test_list.c
	gcc -ansi -pedantic -Wall -c test_list.c
		
list.o: list.h list.c
	gcc -ansi -pedantic -Wall -c list.c
		
clean:
	rm *.o test_list
