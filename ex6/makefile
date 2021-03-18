prog: main.o line_cnt.o 
	gcc -ansi -pedantic -Wall -o prog main.o line_cnt.o
    
main.o: main.c line_cnt.h
	gcc -ansi -pedantic -Wall -c main.c
    
line_cnt.o: line_cnt.c line_cnt.h
	gcc -ansi -pedantic -Wall -c line_cnt.c
