#include <stdio.h>

void swap(int* a, int* b) {
	int c = *a;
	*a = *b;
	*b = c;
}

void sort(int arr[]) {
	int i, j, t;

	for(i = 0; i < 4; i++) {	// �迭 �� - 1 ��ŭ �ݺ�
		for (j = i + 1; j < 5; j++) {	// �ּڰ� ã�Ƽ� 
			if (arr[j] < arr[i]) {
			swap(&arr[j], &arr[i]);			// arr[i]�� �ٲ�ġ�� 
			}
		}
		
	}
}

int main()
{
	int arr[] = {4, 3, 2, 1, 5};
	
	for(int i = 0; i < 5; i++)
		printf("%d ", arr[i]);
	printf("\n");
	
	sort(arr);
	
	for(int j = 0; j < 5; j++)
		printf("%d ", arr[j]);  
}
