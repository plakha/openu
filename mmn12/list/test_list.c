#include <stdio.h>
#include <string.h>

#include "list.h"

int main()
{
	list_t *list = ListCreate();
	
	char *str = "abcde";
	int i = 0;
	
	for (;i < strlen(str); ++i)
	{
		ListPushData(list, str[i]);
	}
	
	return 0;
}
