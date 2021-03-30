letters: letters.o text_state.o 
	gcc -ansi -pedantic -Wall -o letters letters.o text_state.o
    
letters.o: letters.c text_state.h
	gcc -ansi -pedantic -Wall -c letters.c
    
text_state.o: text_state.c text_state.h
	gcc -ansi -pedantic -Wall -c text_state.c
	
clean:
	rm *.o letters
