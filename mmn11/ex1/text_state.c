#include <stdio.h> /* putchar() */
#include <ctype.h> /* isspace() */
#include <assert.h> /* assert() */

#include "text_state.h"

#define FALSE (0)

/*the state machine helps print a char */
/*according to the rules in maman11, exercise 1.*/
/*number is not part of the state machine,*/
/*because they shouldn't be printed at all*/
enum state {
default_state = 0, 
start_sentence_state, 
quotation_state, 
illegal_state
};

/*helps define array of print functions for state machine*/
typedef void (*print_func_t)(int);

/*next state of a state machine is a function of current state and input*/
static enum state NextStatus(enum state current_text_state, int ch);

/**********declarations of functions to print by state ***********/
static void PrintDefault(int ch);
static void PrintStartSentence(int ch);
static void PrintQuotation(int ch);
static void PrintIllegal(int ch); /* error in state machine */

/*is char a dot*/
static int IsDot(int ch);

/*is char a `"` */
static int IsQuot(int ch);

/*array of print functions by state*/
static const print_func_t print_func_arr[] = {
&PrintDefault, 
&PrintStartSentence, 
&PrintQuotation, 
&PrintIllegal
};

static enum state g_current_text_state = start_sentence_state; 

/*print according to the state*/
/*and then find the next state*/
void ProcessChar(int ch)
{
	if (!isdigit(ch))
	{
		(print_func_arr[g_current_text_state])(ch);		
	}
	
	g_current_text_state = NextStatus(g_current_text_state, ch);
}

static enum state NextStatus(enum state current_text_state, int ch)
{
	enum state next_state = illegal_state;
	
	switch (current_text_state)
	{
		case start_sentence_state:
			if (isspace(ch) || (IsDot(ch)))
			{
				next_state = current_text_state;
				assert(next_state == start_sentence_state);
			}
			else if (IsQuot(ch))
			{
				next_state = quotation_state;
			}
			else 
			{
				next_state = default_state;							
			}
			
			break;
			
		case default_state:
			if (IsDot(ch))
			{
				next_state = start_sentence_state;
			}
			else if (IsQuot(ch))
			{
				next_state = quotation_state;
			}
			else
			{
				next_state = current_text_state;
				assert(next_state == default_state);
			}
			
			break;
			
		case quotation_state:
			if (IsQuot(ch))
			{
				next_state = default_state;
			}
			else
			{
				next_state = current_text_state;
				assert(next_state == quotation_state);
			}
			
			break;
			
		case illegal_state:
			assert(FALSE);
			break;
	}

	return next_state;
}	


static void PrintDefault(int ch)
{
	putchar(tolower(ch));
}

static void PrintStartSentence(int ch)
{
	putchar(toupper(ch));
}

static void PrintQuotation(int ch)
{

	putchar(toupper(ch));	
}

static void PrintIllegal(int ch)
{
	fprintf(stderr, "PrintIllegal(%c), line %d\n", ch, __LINE__);
	assert(FALSE);
}

static int IsDot(int ch)
{
	return '.' == ch;
}

static int IsQuot(int ch)
{
	return '\"' == ch;
}

