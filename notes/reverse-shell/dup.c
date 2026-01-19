#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = open("dup.txt", O_RDWR | O_CREAT, S_IRWXU);

    printf("Original fd: %d\n", fd);
    if (fd > 0)
    {
        int cd = dup(fd);
        printf("New fd: %d\n", cd);

        char message[50];
        memset(message, 0x00, sizeof(message));
        strcpy(message, "This is using the copy file descriptor\n");
        write(cd, message, sizeof(message));

        strcpy(message, "This is using the original file descriptor\n");
        write(fd, message, sizeof(message));

        //printf("%d\n", getpid());
        //getchar();

        close(cd);
        close(fd);
    }
    return 0;
}
