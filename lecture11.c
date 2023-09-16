// 구구단 while
// 5단 제외

#include <stdio.h>

int main()
{
	int i, j;
	i = 2;
	j = 1;
	
	while(i <= 9)
	{
		if(i == 5){
			i++;			// 이거 안 하면 5단도 출력됨 
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
