#include <stdio.h>
#include <string.h> /* strchr() */
#include <ctype.h> /* isspace() */
#include <assert.h> /* assert() */

#include "file.h"
#include "mmn14_types.h"

/*
typedef char line_arg_t[MAX_LINE_LEN];
*/

/* enum get_line_status {GOT_LINE, END_OF_FILE, LINE_TOO_LONG}; */
/* TODO don't cut after `"` */
enum get_line_status FileGetLine(FILE *file, char *buf, size_t lim, const char ignore_chars[], int delim)
{
    int ch = EOF;
    size_t i = 0;
    int should_put_delim = 0;
    enum get_line_status status = LINE_TOO_LONG;


    for (; i < (lim - 1)  ; ++i)
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


/* TODO if see ", take all the rest to one arg

TODO check with single arg
     */
dvec_t *FileLineToArgs(const char line[])
{
    static const size_t N_ARGS_EXPECTED = 6;
    char *left = (char *)line;
    char *right = (char *)line;
    char buf[MAX_LINE_LEN] = {'\0'};
    
    dvec_t *args = DVECCreate(sizeof(buf), N_ARGS_EXPECTED);
    if (NULL == args)
    {
        fprintf(stderr, "Cannot allocate memory while running line %d in %s/n", __LINE__, __FILE__);

        return NULL;
    }
    
    while (*left != '\0')
    {
        while (isspace(*left))
        {
            ++left;
        }
        
        if (',' == *left)
        {
            puts(",");
            DVECPushBack(args, strcpy(buf, ","));
            ++left;
            
            continue;
        }
        
        right = left;
        
        while (!isspace(*right) && ',' != *right && '\0' != *right)
        {
            ++right;
        }
        
        buf[right - left] = '\0';
        
        DVECPushBack(args, strncpy(buf, left, right - left));
        
        left = right;
        
    }
    
    return args;
    
}
