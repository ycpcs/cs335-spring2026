#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char data[100];
	memset(data, 0x00, sizeof(data));

	read(STDIN_FILENO, data, sizeof(data));
	write(STDOUT_FILENO, data, sizeof(data));

	return 0;
}