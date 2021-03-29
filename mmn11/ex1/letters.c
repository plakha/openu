#include <stdio.h> /* puts() */

#include "text_state.h" /* ProcessChar() */

int main()
{
    int ch = '\0';
    
    while ( EOF != (ch = getchar()) )
    {
	        ProcessChar(ch);	           
    }
    
    return 0;
}
