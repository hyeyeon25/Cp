#include <stdio.h>

int main() {
	int score[5] = {87, 92, 89, 98, 78};
	
	printf("score[5] = {87, 92, 89, 98, 78} �迭 �ּ� ���\n");
	
	for(int i = 0; i < 5;i++) {
		printf("%d ", &score[i]);
	}
}
