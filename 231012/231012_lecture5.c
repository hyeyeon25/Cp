#include <stdio.h>

void swap(int *pa, int *pb)
{
	int c = *pa;
	*pa = *pb;
	*pb = c;
}

int main() 
{
	int a = 10;
	int b = 55;
	
	printf("a : %d, b : %d\n", a, b);
	swap(&a, &b);
	printf("a : %d, b : %d\n", a, b);
}
