#include <stdio.h>
#include <stdlib.h>

struct Student {
	int sno;
	char name[10];
	int score;
};

int main() {
	int std;
	struct Students* s;
	printf("�л��� �Է�");
	scanf("%d", &std);int
	scores = (int *)malloc(std * sizeof(struct Student));
	
	for(int i = 0; i < std; i++) {
		printf("�л� # %d-%d �й� �Է�", num, i + 1);
		scanf("%d", %s[i].sno);
		//scanf("%d", &scores[i]);
		//scanf("%d", scores + 1)
		printf("�л� # %d-%d �̸� �Է�", num, i + 1);
		scanf("%s", s[i].name, 10);	//�̰� �� & �� ��? 
		printf("�л� # %d-%d ���� �Է�", num, i + 1);
		scanf("%d", %s[i].score);
	}
	
	int sum = 0;
	for ( int i = 0; i < std; i++) {
		sum += s[i].score;
	}
	printf("%d\n", sum);

	
	free(scores);
	return 0;
}
