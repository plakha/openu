#include <stdio.h>
#include <string.h>

#include "list.h"

int main()
{
	list_t *list = ListCreate();
	node_t *node = NULL;
	
	char *str = "abcde";
	int i = 0;
	
	for (;i < strlen(str); ++i)
	{
		puts("Insert");
		putchar(str[i]);
		ListPushData(list, str[i]);
	}
	
	node = ListNodeGetFirst(list);
	puts("");
	do 
	{
		putchar(ListNodeGetData(node));
		node = ListNodeGetNext(node);
	}
	while (!ListNodeIsLast(node));
		
	ListDestroy(list);
	list = NULL;
	
	return 0;
}
