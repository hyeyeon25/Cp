#include <stdio.h>
#include <stdlib.h>

int main()
{
	char ch = 0;
	char str[10];
	
	printf("1. ���� �Է�: ");
	scanf("%c", &ch, sizeof(ch));
	
	printf("2. ���ڿ� �Է�: ");
	scanf("%s", str, sizeof(str));
	
	printf("�Է��� ���� ���: %c\n", ch);
	printf("�Է��� ���ڿ� ���: %s\n", str);
	
	return 0;
}
