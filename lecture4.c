#include <stdio.h>

int main()
{
	int a, b, c, val1, val2, val3, val4, val5;
	a = 0, b = 5, c = 8;
	
	val1 = a && b;
	val2 = a || c;
	val3 = (a > b) && (a < c)
	val4 = !val3;
	val5 = b && c;
	
	printf("a = 0, b = 5, c = 8ÀÏ ¶§\n");
	printf("1. a && b = %d", &val1);
	printf("2. a || c = %d", &val2);
	printf("3. (a > b) && (a < c) = %d", val3);
	printf("4. !val3 = %d", val4);
	printf("5. b && c = %d", val5);
	
	return 0;
}
