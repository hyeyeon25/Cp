#include <stdio.h>

int main()
{
	int year;
	printf("������ �Է��ϼ���. : ");
	scanf("%d", year);
	
	int y = (year % 100 != 0) && (year % 4 == 0) && (year % 400 == 0);
	
	if (y)
	printf("%d���� �����Դϴ�.\n", year);
	
	else
	printf("%d���� ������ �ƴմϴ�.\n", year);
	
	return 0; 
}
