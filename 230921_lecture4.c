#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int points = 1000000;
    int circle = 0;

    srand(time(NULL));

    for (int i = 0; i < points; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        double distance = x * x + y * y;

        if (distance <= 1) {
            circle++;
        }

        if (i % 10000 == 0) {
            double pi = 4.0 * circle / (i + 1);
            double percent = (double)(i + 1) / points * 100.0;
            printf("%.0f%% 진행.. 원주율 : %f \n", percent, pi);
            for (int k = 1; k <= 20; k++) {
                if (k <= (percent * 100) / 500) {
                    printf("■");
                }
                else 
                    printf("□");
                
            }
            printf("\n");
        }
    }

    double pi = 4.0 * circle / points;
    double percent = (double)points / points * 100.0;
    printf("원주율: %f ", pi);

    printf("\n");

    return 0;
}
