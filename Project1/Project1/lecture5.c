#include <stdio.h>

int main(void)
{
	int year;
	printf("������ �Է��ϼ���. : ");
	scanf("%d", &year);
	
//	int y = (year % 100 != 0) && (year % 4 == 0) && (year % 400 != 0);
//	if (y)
	
	if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) 
		printf("%d���� �����Դϴ�.\n", year);
	
	else
		printf("%d���� ������ �ƴմϴ�.\n", year);
	
	return 0;
}


&& and
|| or
