#include <stdio.h>

int main()
{
	int sel;
	
	printf("1.���ϱ� 2.���� 3.���ϱ� 4.������ : ");
	scanf("%d", &sel);
	
	calculator(sel);
	
	return 0;
}

void calculator(int selector)
{
	double num1, num2;
	
	if(selector < 1 || selector > 4)
	{
		printf("����!!");
		return;
	}
	
	printf("���� 1�� �Է��Ͻÿ� : ");
	scanf("%lf", &num1);
	
	printf("���� 2�� �Է��Ͻÿ� : ");
	scanf("%lf", &num2);
	
	if (selector == 1) sum(num1, num2);
	else if (selector == 2) sum(num1, num2);
	else if (selector == 3) sum(num1, num2);
	else if (selector == 4) sum(num1, num2);

}
