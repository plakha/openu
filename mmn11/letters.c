#include <stdio.h> /* */
#include "text_state.h"

int main()
{
    int ch = '\0';
    size_t cnt_line = 0;
    
    while ( EOF != (ch = getchar()) )
    {
	        ProcessChar(ch);	           
    }

  
   
    printf("A count: %ld\n", cnt_line);
    
    return 0;
}
