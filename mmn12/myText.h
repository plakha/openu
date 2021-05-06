#ifndef MY_TEST_H
#define MY_TEST_H

#ifndef LINE_LEN
#define LINE_LEN (60)
#endif

#include <stdio.h>

#include "./list/list.h"
#include "./buffer/buffer.h"

enum data_structure {buffer, list};

const char *CHARS_TO_NOT_READ = "\n";

/* receive text and store it data structure of your choice: 0 - dynamic buffer, 1 - linked list.
Returns pointer to data structure
new line character is not stored.
Storing '\0' may resuly in undefined behavior
*/
void *readText(enum data_structure ds_kind);


/*receives the pointer to the teh data based that was received from readText(), 
and the enum that tells which data structure this is.
prints the text in lines of LINE_LEN
*/
void printText(void *ds, enum data_structure ds_kind);







#endif /* MY_TEST_H */
