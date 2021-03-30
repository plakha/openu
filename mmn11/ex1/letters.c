#include <stdio.h> /* puts() */

#include "text_state.h" /* ProcessChar() */

/*the program receives text from standard input */
/*and prints moodifies text to standard output.*/
/*The rules for modification are specified in maman11, ex1*/
int main()
{
    int ch = '\0';
    
    while ( EOF != (ch = getchar()) )
    {
	        ProcessChar(ch);	           
    }
    
    return 0;
}
