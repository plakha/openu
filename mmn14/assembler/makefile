test_assembler: test_assembler.o assembler.o
	gcc -ansi -pedantic -Wall -o test_assembler test_assembler.o assembler.o
		
test_assembler.o: assembler.h test_assembler.c
	gcc -ansi -pedantic -Wall -c test_assembler.c
		
assembler.o: assembler.h assembler.c
	gcc -ansi -pedantic -Wall -c assembler.c
		
clean:
	rm *.o test_assembler
