/* print_zero.c  */
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
   char src[40] = "Hello CS335 \0 Can you see this\n";
   char dest[40];
   memset(dest, 0x00, sizeof(dest));

   strcpy(dest, src);

   printf("%s", dest);
}
