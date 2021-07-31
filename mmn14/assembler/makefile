CC = gcc
CCFLAGS = -ansi -pedantic -Wall


#if building in release(all) after having built in debug, with no changes in source
#run make clean first
all: global.out

#if building in debug after having built in release (all), with no changes in source
#run make clean first
debug:  CCFLAGS += -DDEBUG -g3 -O0
debug: global.out

assembler: assembler.o
	$(CC) $(CCFLAGS) -o assembler assembler.o
		

assembler.o: assembler.c assembler.h
	$(CC) $(CCFLAGS) -c assembler.c
		
clean:
	rm *.o assembler
