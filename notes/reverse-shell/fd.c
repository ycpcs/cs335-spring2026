#include <stdio.h>
#include <fcntl.h>  // file control options
#include <unistd.h> // posix operating system API (Portable Operating System Interface)
#include <string.h>

void main()
{
    int fd = open("test.txt", O_RDWR | O_CREAT, S_IRWXU);
    // S_IRWXU - 00700 user (file owner) has read, write, and execute permission

    printf("File Descriptor: %d\n", fd);

    if (fd > 0)
    {
        char *data = "Learning about descriptors!\n";
        write(fd, data, strlen(data));
        close(fd);
    }

    // printf("Printing the Standard file descriptors:\n");

    // printf("Standard in: %d\n", STDIN_FILENO);
    // printf("Standard out: %d\n", STDOUT_FILENO);
    // printf("Standard error: %d\n", STDERR_FILENO);

    // printf("%d\n", getpid());
    // getchar();
}