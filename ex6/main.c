#include <stdio.h> /* */
#include "line_cnt.h"

int main()
{
    int ch = '\0';
    size_t cnt_line = 0;
    
    while ( EOF != (ch = getchar()) )
    {
        do_nothing();
        if ('\n' == ch)
        {
            puts(MESSAGE);
            ++cnt_line;
        }
    }

  
   
    printf("A count: %ld\n", cnt_line);
    
    return 0;
}
