#include <string.h> /* strchr() */
#include <assert.h> /* assert() */
#include <stdlib.h> /* size_t */
#include <stdio.h> /* printf() */
#include "myText.h"

enum {SUCCESS, FAILURE};
enum {FALSE};

int main()
{
	int ds_kind = -1;
	void *ds = NULL;
	
	printf("The program receives standard input and prints it back by length of %d\n", LINE_LEN);
	puts("Please enter 0 to use buffer for storage, or 1 - to use linked list");
	
	while (ds_kind < buffer || list < ds_kind)
	{	
		scanf("%d", &ds_kind);
	}
	
	puts("Please enter the text\n*****************");
	
	ds = readText(ds_kind);
	
	puts("The text has been received\n***************");
	
	printText(ds, ds_kind);
	
/*	Clean the memory*/

	switch (ds_kind)
	{
		case buffer:
			BufDestroy(ds);
			
			break;
		
		case list:
			ListDestroy(ds);
			
			break;
			
		default:
			assert(FALSE);
			
			break;
	}
	ds = NULL;
	
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
			
		default:
			assert(FALSE);
			
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
	node_t *node = NULL;
	size_t index = 0;
	
	switch (ds_kind)
	{
		case buffer:
			for (index = 0; BufSize(ds) > index; ++index)
			{
				if ((0 == index % LINE_LEN) && (0 != index)) {puts("");}
				
				putchar(BufGetItem(ds, index));			
			}
			
			break;
	
		case list:
			if (ListIsEmpty(ds)) {break;}
			node = ListNodeGetFirst(ds);
			
			do
			{
				if ((0 == index % LINE_LEN) && (0 != index)) {puts("");}
				
				putchar(ListNodeGetData(node));
				node = ListNodeGetNext(node);
				++index;
			}
			while (!ListNodeIsLast(node));
			
			break;
		
		default:
			assert(FALSE);
			
			break;	
	}

}






