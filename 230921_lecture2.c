#include <stdio.h>

int main()
{
	int sel;
	
	printf("1.더하기 2.빼기 3.곱하기 4.나누기 : ");
	scanf("%d", &sel);
	
	calculator(sel);
	
	return 0;
}

void calculator(int selector)
{
	double num1, num2;
	
	if(selector < 1 || selector > 4)
	{
		printf("오류!!");
		return;
	}
	
	printf("숫자 1을 입력하시오 : ");
	scanf("%lf", &num1);
	
	printf("숫자 2를 입력하시오 : ");
	scanf("%lf", &num2);
	
	if (selector == 1) sum(num1, num2);
	else if (selector == 2) sum(num1, num2);
	else if (selector == 3) sum(num1, num2);
	else if (selector == 4) sum(num1, num2);

}
