prog: test_buffer.o buffer.o
	gcc -ansi -pedantic -Wall -o test_buffer test_buffer.o buffer.o
		
test_buffer.o: buffer.h test_buffer.c
	gcc -ansi -pedantic -Wall -c test_buffer.c
		
buffer.o: buffer.h buffer.c
	gcc -ansi -pedantic -Wall -c buffer.c
		
clean:
	rm *.o test_buffer
