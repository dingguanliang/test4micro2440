#include <types.h>
#include <stdio.h>
#include <dev_test.h>

int32_t main()
{
	uint8_t i, j;

	puts("\n\r--------- Welcome to Ding Guanliang world! ---------\n\r");

	for(i = 0; test_t[i].test_func != NULL; i++)
		printf("%d - %s\n\r", i, test_t[i].head);
	puts("input a item: ");
	j = getc() & 0x0f;
	if(j < i)
	{
		printf("%d\n\r", j);
		test_t[j].test_func();
	}
	else
		puts("\n\r");
	
	puts("----------------------------------------------------\n\r");
	puts("input any key to continue!\n\r");
	getc();
	
	return(0);
}
