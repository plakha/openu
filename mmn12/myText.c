#include <string.h> /* strchr() */
#include <assert.h> /* assert() */
#include <stdlib.h> /* size_t */

#include "myText.h"

enum {SUCCESS, FAILURE};

int main()
{
	return 0;

}

void *readText(enum data_structure ds_kind)
{
	int ch = '\0';
	
	void *ret_ds = NULL;
	int status = SUCCESS;
	
	switch (ds_kind)
	{
		case buffer:
			ret_ds = BufCreate();
			
			break;
		
		case list:
			ret_ds = ListCreate();
			
			break;
	}
			
	if (!ret_ds)
	{
		puts("ERROR: Memory could not be allocated to store any input");
		
		return NULL;
	}
    
    while ((SUCCESS == status) && (EOF != (ch = getchar())))
    {
    	if (strchr(CHARS_TO_NOT_READ, ch)) {continue;}
    	
		switch (ds_kind)
		{
			case buffer:
				status  = BufPush(ret_ds, ch);
				
				break;
			
			case list:
				status = ListPushData(ret_ds, ch);
				
				break;
			
			default:
				assert(FALSE);
				
				break;
    	}
    }
    
    if (SUCCESS != status)
    {
    	puts("ERROR: Memory could not be allocated, Some input wasnsn't read");
    }
    
	return ret_ds;
}

void printText(void *ds, enum data_structure ds_kind)
{
	char ch = '\0';
	size_t
	
	switch (ds_kind)
	{
		case buffer:
			
			break;
	
		case list:
			
			break;
		
		default:
			assert(FALSE);
			
			break;	
	}
}






