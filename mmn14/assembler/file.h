#ifndef FILE_H
#define FILE_H

#include <stdlib.h> /* size_t */

#include "dvec.h"


enum get_line_status {GOT_LINE, END_OF_FILE, LINE_TOO_LONG};


/*
    the function reads one line from file to buf
    replace contingent ignore-chars with one instance of delim char
    the line has to be less than MAX_LINE (considering the ignored chars are replaced with the delim char)

    if the whole line has been read up to new_line - returns GOT_LINE
    if the whole last line had been read - up to EOF -  returns END_OF_FILE
    if the line was equal or longer then lim - returns LINE_TOO_LONG
*/
enum get_line_status FileGetLine(FILE *file, char *buf, size_t lim, const char ignore_chars[], int delim);


/* Receves the line and returns arg vector. Note, commas are considered arguments for this purpose
    e.g, upon receiving "move $1, $2", returns vector that contains: "move", "$1", ",", "$2"
    This is to help parser validate the arguments
*/
dvec_t *FileLineToArgs(const char line[]);

/*
    Frees the argument vector
*/
void FileFreeArgs(dvec_t *dvec);

#endif /* FILE_H */
