#include <stdio.h>
void swap(int *p)
{
   int i, j, cnt;
   for (i = 0; i < 5 - 1; i++) {
      for (j = i + 1; j < 5; j++) {
         if (*(p + i) > *(p + j)) {
            cnt = *(p + i);
            *(p + i) = *(p + j);
            *(p + j) = cnt;
         }
      }
   }
}
int main()
{
   int arr[5] = { 5, 7, 2, 3, 4 };
   int* p_arr = arr[5];
   int cnt;
   for (int i = 0; i < 5; i++)
   {
      printf("%d ", arr[i]);
   }
   
   swap(arr);
   for (int i = 0; i < 5 - 1; i++) {
      for (int j = i + 1; j < 5; j++) {
         if (arr[i] > arr[j]) {
            cnt = arr[i];
            arr[i] = arr[j];
            arr[j] = cnt;
         }
      }
   }
   printf("\n");
   for (int i = 0; i < 5; i++)
   {
      printf("%d ", arr[i]);
   }
}
