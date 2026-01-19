#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int in = open("/etc/passwd", O_RDONLY); // need a valid file
    int out = open("output.txt", O_RDWR | O_CREAT, S_IRWXU);

    if (in > 0 && out > 0)
    {
        printf("in: %d\n", in);
        printf("out: %d\n", out);

        dup2(in, STDIN_FILENO);
        dup2(out, STDOUT_FILENO);

        char message[50];
        memset(message, 0x00, sizeof(message));

        fgets(message, sizeof(message), stdin);
        printf("%s\n", message);

        // printf("%d\n", getpid());
        // getchar();

        close(in);
        close(out);
    }
    return 0;
}
