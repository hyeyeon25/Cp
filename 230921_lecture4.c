#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main()
{
	int rolling = pow(10,9);
	double x = 0;
	double y = 0;
	double z = 0;
	
	int pick = pow(10,7);
	int stack = 1;
	
	int circle = 0;
	double k;
	double result;
	srand(time(NULL));
	
	for(int a = 1; a <= rolling; a++)
	{
		x = (double)rand() / (double)RAND_MAX;
		y = (double)rand() / (double)RAND_MAX;
		z = x*x + y*y;
		if(z <= 1)
		{
			circle++;
		}
		if(a % pick == 0)
		{
			k = (double)circle / a;
			result = (double)k * 4;
			printf("%d%% ����, ���� ������: %f\n",stack,result);
			stack++;
		}
	}
	printf("���� ������: %f", result);
}
