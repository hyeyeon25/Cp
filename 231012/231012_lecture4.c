#include <stdio.h>

int main()
{
	int score[5] = {87, 92, 94, 65, 33};
	int cnt;
	
	int* p_score = score + 1;
	
	for (cnt = 0; cnt < 3; cnt++) {
		printf("%d\n", score[cnt]);
	}
	return 0;
}
// int형 포인터라 4바이트씩 추가 
