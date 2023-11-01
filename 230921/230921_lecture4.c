#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int points = 1000000000;
    int circle = 0;

    srand(time(NULL));

    for (int i = 1; i < points; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        
    double distance = x * x + y * y;

    if (distance <= 1) {
        circle++;
    	}

    if (i % 10000000 == 0) {
        double pi = 4.0 * circle / (i);
        double percent = (double)(i) / points * 100.0;
        printf("%2.f%% ����.. ������ : %f \n", percent, pi);
        for (int k = 1; k <= 20; k++) {
            if (k <= percent / 5) {
                printf("��");
            	}
            else 
                printf("��");
        		}
        	
            printf("\n");
        	}
    }

    double pi = 4.0 * circle / points;
    printf("���� ������: %f ", pi);

    printf("\n");

    return 0;
}

//%(��ü �ڸ���).0(�Ҽ��� ���� ��ǥ��)f(progress)%(�ۼ�Ʈ ǥ��)%(�̰� �ϳ��� ����? �ٵ� �� ����� ������� %�� ����)
