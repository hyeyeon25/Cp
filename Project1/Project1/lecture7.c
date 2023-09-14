#include <stdio.h>

int main()
{
	int js;
	
	scanf("%d", js);
	
	if(js >= 90)
	printf("A");
	
	else if(js >= 80 && js <= 90)
	printf("B");
	
	else if(js >= 70 && js <= 80)
	printf("C");
	
	else if(js >= 60 && js <= 70)
	printf("D");
	
	else if(js >= 50 && js <= 60)
	printf("E");
	
	else(js > 100 || js < 0)
	printf("잘못 입력하였습니다.\n");
	
	return 0;
}
