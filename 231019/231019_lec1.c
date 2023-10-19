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
	printf("학생수 입력");
	scanf("%d", &std);int
	scores = (int *)malloc(std * sizeof(struct Student));
	
	for(int i = 0; i < std; i++) {
		printf("학생 # %d-%d 학번 입력", num, i + 1);
		scanf("%d", %s[i].sno);
		//scanf("%d", &scores[i]);
		//scanf("%d", scores + 1)
		printf("학생 # %d-%d 이름 입력", num, i + 1);
		scanf("%s", s[i].name, 10);	//이건 왜 & 안 씀? 
		printf("학생 # %d-%d 성적 입력", num, i + 1);
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
