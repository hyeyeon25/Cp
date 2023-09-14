#include <stdio.h>

int main()
{
	int i, j;
	i = 2;
	j = 1;
	
	while(i <= 9){
		if(i == 5) {
			i++;			// 이거 안 하면 5인 상태로 계속 돌아감 
		continue;		
	}
		
		while(j <= 9){
		printf("%d * %d = %d\n", i, j, i * j);
		j++;
	}
	i++;
	j = 1;
	}
	return 0;
}
