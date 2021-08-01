#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>

/*
Assemblers receives RAM-table on its creation.
This module receives lines (strings) and pushes them to RAM.

TODO
1. Allocate RAM (consider creating distinct module for RAM).
2. 





*/

void ParseStatement(char *statement[]);

#endif /* ASSEMBLER_H */