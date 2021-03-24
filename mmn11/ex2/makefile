my_sin.out: my_sin.o
	gcc -ansi -pedantic -Wall -o my_sin.out my_sin.o -lm
    
my_sin.o: my_sin.c my_sin.h
	gcc -ansi -pedantic -Wall -c my_sin.c -lm
	
clean:
	rm *.o *out
