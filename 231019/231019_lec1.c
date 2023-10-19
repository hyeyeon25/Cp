/*
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
	scanf("%d", &std);
	scores = (int *)malloc(std * sizeof(struct Student));
	
	for(int i = 0; i < std; i++) {
		printf("학생 # %d-%d 학번 입력", std, i + 1);
		scanf("%d", %s[i].sno);
		//scanf("%d", &scores[i]);
		//scanf("%d", scores + 1)
		printf("학생 # %d-%d 이름 입력", std, i + 1);
		scanf("%s", s[i].name, 10);	//이건 왜 & 안 씀? => 문자열이라 
		printf("학생 # %d-%d 성적 입력", std, i + 1);
		scanf("%d", %s[i].score);
	};
	printf("총점: %d\n", sum)
	
	int sum = 0;
	for (int i = 0; i < std; i++) {
		sum += s[i].score;
	}
	printf("%d\n", sum);

	free(scores);
	
	return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>

struct student {
   int sno;
   int name[10];
   int score;
};

int main() {
   int* score;
   int count = 0;
   int sum = 0;
   float p = 0;
   
   printf("학생 수 입력 : ");
   scanf("%d", &count);
   score = (int*)malloc(10 * sizeof(int));



   for (int i = 0;i < count;i++) {
      
      printf("학생 # %d-%d 성적 입력", count, i+1);
      scanf("%d", score+i);
      sum += score[i];
   }
   printf("총점 : %d\n", sum);

   for (int k = 0;k < count;k++) {
      printf("학생 # %d-%d 성적 출력 : %d \n ", count, k+1,score[k]);


   }
   
   p = (float)sum / count;
   printf("평균 점수 : %.2f", p);
   free(score);

   return 0;

}
