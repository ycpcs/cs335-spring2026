#include <string.h>
#include <stdio.h>

void main()
{
    char src[16]="Hello World";
    char dest[16];
    
    strcpy(dest, src);

    printf("src: %s\n", src);
    printf("dest: %s\n", dest);
}
