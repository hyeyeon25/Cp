#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int points = 1000000000;
    int circle = 0;  

    srand(time(NULL));

    for (int i = 0; i <= points; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
		//랜덤하게 다트 던짐
        double distance = x * x + y * y;

        if (distance <= 1) {
            circle++;
        }

        if (i % 10000000 == 0) {
            double pi = 4.0 * circle / (i + 1); 
            double percent = (double)(i + 1) / points * 100.0;
            printf("%2.f%% 진행.. 원주율 : %f \n", percent, pi);
            for (int k = 1; k <= 20; k++) {
                if (k <= percent / 5) {
                    printf("■");
                }
                else 
                    printf("□");
                
            }
            printf("\n");
        }
    }

    double pi = 4.0 * circle / points;
    printf("최종 원주율: %f ", pi);

    printf("\n");

    return 0;
}

//%(전체 자릿수).0(소수점 이하 미표시)f(progress)%(이거 하나는 뭐지?)%(찐 퍼센트)
