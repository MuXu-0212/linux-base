#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char buf[10];
	int flags, n;

	flags = fcntl(STDIN_FILENO, F_GETFL);	//F_GETFL获取stdin属性信息

	if (flags == -1)
	{
		perror("fcntl error");
		exit(1);
	}

	flags |= O_NONBLOCK;
	int ret = fcntl(STDIN_FILENO, F_SETFL, flags);

	if (ret == -1)
	{
		perror("fcntl error");
		exit(1);
	}

tryagain:
	n = read(STDIN_FILENO, buf, 10);
	if (n < 0)
	{
		if (errno != EAGAIN)
		{
			perror("read /dev/tty");
			exit(1);
		}
		sleep(3);
		write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
		goto tryagain;
	}
	write(STDOUT_FILENO, buf, n);

	return 0;
}