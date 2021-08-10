#include <stdio.h>
#include <string.h> /* strchr() */

#include "file.h"

/* enum get_line_status {GOT_LINE, END_OF_FILE, LINE_TOO_LONG}; */
/* TODO don't cut after `"` */
enum get_line_status FileGetLine(FILE *file, char *buf, size_t lim, const char ignore_chars[], int delim)
{
    int ch = '\0';
    size_t i = 0;
    int should_put_delim = 0;
    enum get_line_status status = LINE_TOO_LONG;


    for (; i < lim - 1  ; ++i)
    {
        ch = fgetc(file);

        if (EOF == ch)
        {
            status = END_OF_FILE;

            break;
        }
        else if ('\n' == ch)
        {
            status = GOT_LINE;
                
            break;  
        }

        if (strchr(ignore_chars, ch))
        {
            should_put_delim = 1;
            --i;

            continue;
        }
    
        if (should_put_delim)
        {
            buf[i] = delim;
            should_put_delim = 0;
            ++i;
        }

        buf[i] = ch;
        
    }

    if ('\n' == ch && NULL == strchr(ignore_chars, ch))
    {
        buf[i] = ch;
        ++i;
    }

    buf[i] = '\0';

    return status;
}
