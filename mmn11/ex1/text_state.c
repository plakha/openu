#include <stdio.h> /* putchar() */
#include <ctype.h> /* */
#include <assert.h> /*assert()*/


enum state {default_state = 0, start_sentence_state, quotation_state, illegal_state};

typedef void (*print_func_t)(int);

static enum state g_current_text_state = start_sentence_state; 

/********static functions declaration*****************/
static enum state NextStatus(enum state current_text_state, int ch);



static void print_default(int ch);
static void print_start_sentence(int ch);

/********************end static functions declaration****************/

static const print_func_t print_func_arr[] = {&print_default, &print_start_sentence, NULL, NULL};

void ProcessChar(int ch)
{
	printf("state enum %d, line %d", g_current_text_state, __LINE__);
/*	(print_func_arr[g_current_text_state])(ch);*/
	if (!isdigit(ch))
	{
			
	}
	
	g_current_text_state = NextStatus(g_current_text_state, ch);
}

static enum state NextStatus(enum state current_text_state, int ch)
{
	enum state next_state = illegal_state;
	
	switch (current_text_state)
	{
		case start_sentence_state:
			next_state = default_state;
			break;
			
		case default_state:
			break;
			
		case quotation_state:
			break;
			
		case illegal_state:
			
			assert(0);
			break;
	}

	return default_state;	
/*	return next_state;*/
}	

/**********definitions print by state ***********/

static void print_default(int ch)
{
	puts("print_default");
	putchar(tolower(ch));
}

static void print_start_sentence(int ch)
{
	puts("print_start_sentence");
	putchar(toupper(ch));
}



/*********************************************************/

/*
* remember TO REMOVE
*/
void do_nothing()
{
    puts("Nothing done.");
}

