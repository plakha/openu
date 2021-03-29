#include <stdio.h> /* putchar() */
#include <ctype.h> /* isspace() */
#include <assert.h> /* assert() */

#define FALSE (0)

enum state {default_state = 0, start_sentence_state, quotation_state, illegal_state};

typedef void (*print_func_t)(int);


/********static functions declaration*****************/
static enum state NextStatus(enum state current_text_state, int ch);

static void PrintDefault(int ch);
static void PrintStartSentence(int ch);
static void PrintQuotation(int ch);
static void PrintIllegal(int ch); /* error in state machine */

static int IsDot(int ch);
static int IsQuot(int ch);
/********************end static functions declaration****************/

static const print_func_t print_func_arr[] = {&PrintDefault, &PrintStartSentence, &PrintQuotation, &PrintIllegal};
static enum state g_current_text_state = start_sentence_state; 


void ProcessChar(int ch)
{
/*	printf("state enum %d, line %d\n", g_current_text_state, __LINE__);*/
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
			break;
	}

	return next_state;
}	

/**********definitions print by state ***********/

static void PrintDefault(int ch)
{
/*	puts("print_default");*/
	putchar(tolower(ch));
}

static void PrintStartSentence(int ch)
{
/*	puts("print_start_sentence");*/
	putchar(toupper(ch));
}

static void PrintQuotation(int ch)
{
/*	puts("print_quotation");*/
	putchar(toupper(ch));	
}
static void PrintIllegal(int ch)
{
	fprintf(stderr, "print_illegal(%c)\n", ch);
}




static int IsDot(int ch)
{
	return '.' == ch;
}

static int IsQuot(int ch)
{
	return '\"' == ch;
}

/*
* remember TO REMOVE
*/
/*void do_nothing()*/
/*{*/
/*    puts("Nothing done.");*/
/*}*/

