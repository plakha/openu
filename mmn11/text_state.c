#include <stdio.h> /* putchar() */
#include <ctype.h> /* */

void do_nothing()
{
    puts("Nothing done.");
}

enum state {default_state, start_sentence};

static enum state text_state = start_sentence; 

void process_char(int ch)
{
	switch (text_state)
	{
		case start_sentence:
			putchar (toupper(ch));
			text_state = default_state;
			break;
		default:
			putchar(ch);
			if ('.' == ch)
			{
				text_state = start_sentence;
			}
				
	}
}
