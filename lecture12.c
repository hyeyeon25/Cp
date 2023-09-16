#include <stdio.h>
int main()
{
   int i, j;
   for (i = 1; i <= 5; i++)
   {
      for (j = 1; j < i; j++)
      {
         printf("   ");
      }
      if (i % 2 == 0) printf("| X |\n");
      else printf("| O |\n");
      printf("\n----------------------\n");
   }
   return 0;
}
