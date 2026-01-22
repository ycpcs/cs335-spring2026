#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
	setuid(getuid());
	system("/bin/ls -l");
}
