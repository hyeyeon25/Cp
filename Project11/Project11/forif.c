#include <stdio.h>

int main()
{
	int floor = 0;
	printf("¸î ÃþÀ¸·Î ½×°Ú½À´Ï±î?");
	scanf_s("%d", &floor);

	for (int i = 0; i < floor; i++)
	{
		for (int j = i; j < floor - 1; j++)
		{
			printf(" ");
		}

		for (int k = 0; k < i * 2 + 1; k++)
		{
			printf("*");
		}
		printf("\n");
	}


	return 0;
}