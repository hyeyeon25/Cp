#include <stdio.h>

int main()
{
	int js, mok;
	
	scanf("%d", js);
	
	mok = js / 10;
	
	if(js > 100 || js < 0)
	printf("�߸� �Է��Ͽ����ϴ�.\n");
	return 0;
	
	switch(js){
	case 9:
		printf("A");
		break;
		
	case 8:
		printf("B");
		break;
		
	case 7:
		printf("C");
		break;
		
	case 6:
		printf("D");
		break;
		
	default:
		printf("F");
		break;
	}
}
