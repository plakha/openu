#include <string.h>
#include <stdio.h>

#include "barr.h"

int main()
{

	barr_t *my_barr = BArrCreate();
	
	
	printf("%ld\n", strlen((char *)my_barr));
	BArrDestroy(my_barr);
	
	return 0;
}
