#include <stdio.h>

int main()
{
	int year;
	printf("연도를 입력하세요. : ");
	scanf("%d", year);
	
	int y = (year % 100 != 0) && (year % 4 == 0) && (year % 400 == 0);
	
	if (y)
	printf("%d년은 윤년입니다.\n", year);
	
	else
	printf("%d년은 윤년이 아닙니다.\n", year);
	
	return 0; 
}
