#include <stdio.h>

int isPrime(int num)
{
	for(int div = num - 1 ; div > 1; div--)
	{
		if (num % div ==0) return 0;
	}
	return 1;
}



int main()
{
	int num;
	printf("�Ҽ����� Ȯ���� ���� �Է��ϼ���. :");
	scanf("%d", &num);
	printf("%d\n", isPrime(num));
}
