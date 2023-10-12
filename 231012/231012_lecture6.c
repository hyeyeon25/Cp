#include <stdio.h>

void swap(int* a, int* b)
{
   int c = *a;
   *a = *b;
   *b = c;
}


void sort(int *arr) {
   for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4 - i; j++) {
         if (arr[j] > arr[j + 1]) {
            swap(&arr[j], &arr[j + 1]);
         }
      }
   }
}

int main() {
   int arr[5] = {2, 5, 4, 1, 3};
   for (int i = 0; i < 5; i++) {
   	printf("%d ", arr[i]);
   }
   printf("\n");
	
   sort(arr);
	for (int j = 0; j < 5; j++)
    	printf("%d ", arr[j]);
}
