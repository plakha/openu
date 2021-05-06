prog: myText.o buffer.o list.o
	gcc -ansi -pedantic -Wall -o myText myText.o buffer.o list.o

myText.o: myText.h myText.c
	gcc -ansi -pedantic -Wall -c myText.c

list.o: ./list/list.h ./list/list.c
	gcc -ansi -pedantic -Wall -c ./list/list.c
		
buffer.o: ./buffer/buffer.h ./buffer/buffer.c
	gcc -ansi -pedantic -Wall -c ./buffer/buffer.c
	
clean:
	rm *.o myText;
