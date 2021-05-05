#include <stdio.h>
#include <string.h>
#include "buffer.h"


int main()
{
	
	char *str = "abcdef";
	int i = 0;
	
	buffer_t *my_buf = BufCreate();

	for (; i < strlen(str); ++i)
	{
		BufPush(my_buf, str[i]);
	}
	
	for (i = 0; i < strlen(str); ++i)
	{
		putchar(BufGetItem(my_buf, i));
	}

	BufDestroy(my_buf);
		
	return 0;
}
