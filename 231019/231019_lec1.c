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
	printf("�л��� �Է�");
	scanf("%d", &std);
	scores = (int *)malloc(std * sizeof(struct Student));
	
	for(int i = 0; i < std; i++) {
		printf("�л� # %d-%d �й� �Է�", std, i + 1);
		scanf("%d", %s[i].sno);
		//scanf("%d", &scores[i]);
		//scanf("%d", scores + 1)
		printf("�л� # %d-%d �̸� �Է�", std, i + 1);
		scanf("%s", s[i].name, 10);	//�̰� �� & �� ��? => ���ڿ��̶� 
		printf("�л� # %d-%d ���� �Է�", std, i + 1);
		scanf("%d", %s[i].score);
	};
	printf("����: %d\n", sum)
	
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
   
   printf("�л� �� �Է� : ");
   scanf("%d", &count);
   score = (int*)malloc(10 * sizeof(int));



   for (int i = 0;i < count;i++) {
      
      printf("�л� # %d-%d ���� �Է�", count, i+1);
      scanf("%d", score+i);
      sum += score[i];
   }
   printf("���� : %d\n", sum);

   for (int k = 0;k < count;k++) {
      printf("�л� # %d-%d ���� ��� : %d \n ", count, k+1,score[k]);


   }
   
   p = (float)sum / count;
   printf("��� ���� : %.2f", p);
   free(score);

   return 0;

}
