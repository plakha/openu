CC = gcc
CCFLAGS = -pedantic -Wall -DNDEBUG
DEBUGCCFLAGS = -pedantic -Wall -g3 -O0
SOURCES=assembler.c dll.c dvec.c file.c parser.c ram.c sym_tab.c
HEADERS=$(SOURCES:.c=.h) 
HEADERS+=mmn14_types.h
OBJECTS=$(SOURCES:.c=.o)
OBJDIR=obj
EXECUTABLE=assembler


#if building in release(all) after having built in debug, with no changes in source
#then run make clean first
all: $(EXECUTABLE)

#if building in debug after having built in release (all), with no changes in source
#then run make clean first
debug:  CCFLAGS=$(DEBUGCCFLAGS)
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^
		
%.o: %.c $(HEADERS)
	$(CC) $(CCFLAGS) -ansi -c $<

.PHONY: clean
		
clean:
	rm *.o assembler
