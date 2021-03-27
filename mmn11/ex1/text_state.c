#include <stdio.h> /* putchar() */
#include <ctype.h> /* */
#include <assert.h> /*assert()*/

#define FALSE (0)

enum state {default_state = 0, start_sentence_state, quotation_state, illegal_state};

typedef void (*print_func_t)(int);


/********static functions declaration*****************/
static enum state NextStatus(enum state current_text_state, int ch);

static void print_default(int ch);
static void print_start_sentence(int ch);
static void print_quotation(int ch);
static void print_illegal(int ch);
/********************end static functions declaration****************/

static const print_func_t print_func_arr[] = {&print_default, &print_start_sentence, &print_quotation, &print_illegal};
static enum state g_current_text_state = start_sentence_state; 


void ProcessChar(int ch)
{
	printf("state enum %d, line %d\n", g_current_text_state, __LINE__);
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
			if (!isspace(ch))
			{
				next_state = default_state;			
			}
			
			break;
			
		case default_state:
			break;
			
		case quotation_state:
			break;
			
		case illegal_state:
			break;
	}

	return next_state;
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

static void print_quotation(int ch)
{
	puts("print_quotation");
	putchar(toupper(ch));	
}
static void print_illegal(int ch)
{
	fprintf(stderr, "print_illegal(%c)\n", ch);
}

/*********************************************************/

/*
* remember TO REMOVE
*/
void do_nothing()
{
    puts("Nothing done.");
}

