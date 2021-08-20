#ifndef FILE_H
#define FILE_H

#include <stdlib.h> /* size_t */

#include "dvec.h"


enum get_line_status {GOT_LINE, END_OF_FILE, LINE_TOO_LONG};


/*
    the function reads one line (up to lim exclusive) from file to buf
    replace contingent ignore-chars with one instance of delim char
    the line has to be less than MAX_LINE (considering the ignored chars are replaced with the delim char)

    if the whole line has been read up to new_line - returns GOT_LINE
    if the whole last line had been read - up to EOF -  returns END_OF_FILE
    if the line was equal or longer then lim - returns LINE_TOO_LONG
*/
enum get_line_status FileGetLine(FILE *file, char *buf, size_t lim, const char ignore_chars[], int delim);


/* if prev status was LINE_TOO_LONG - do not promote, because we are in the same line still. The line is to be ignored beyond MAX_LINE_LEN */
/*
oid FilePromoteLineCounter(size_t *input_line_count, enum get_line_status prev_line_status);
*/

dvec_t *FileLineToArgs(const char line[]);

void FileFreeArgs(dvec_t *dvec);


/* TODO
1. Maybe keep the whole text in a vector of buf[MAX_LINE]? Reuse buffer from mman 12 (accomodate to type char[MAX_LINE]) It may be easier to map. Why? All the symbols will be in one place, so the memory can be freed in the end.
2. where should I put the code to disregard comments (line that starts with ';')
3. disregard empty lines (that contain only spacenew line characters) - my GetLine finc is supposed to put one single delimiter in place of multiple ignorable chars,
 but it is safer to make sure that for all the chars, isspace() returns true 



 4. all string characters should be kept inside "" after .asciz
*/
#endif /* FILE_H */
