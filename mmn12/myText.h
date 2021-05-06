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

void *readText(enum data_structure ds_kind);

void printText(void *ds, enum data_structure ds_kind);







#endif /* MY_TEST_H */
