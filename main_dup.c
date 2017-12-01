#include "stdio.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char buf[128];

int f1()
{
	int res = 0;
	int log_fd = 0;
	int null_fd = 0;

	log_fd = open("log", O_RDWR | O_CREAT, 0664);
	printf("log_fd:%d\n", log_fd);
	res = write(log_fd, "*****\n", 6);
	printf("log_fd write res:%d\n", res);

	null_fd = open("/dev/null", O_RDWR);

	// dup
	res = dup2(null_fd, 0);
	sprintf(buf, "dup2 0 res:%d\n", res);
	write(log_fd, buf, strlen(buf));

	res = dup2(null_fd, 1);
	sprintf(buf, "dup2 1 res:%d\n", res);
	write(log_fd, buf, strlen(buf));

	res = dup2(null_fd, 2);
	sprintf(buf, "dup2 2 res:%d\n", res);
	write(log_fd, buf, strlen(buf));

	// write
	res = write(1, "hi\n", 3);
	sprintf(buf, "write 1 res:%d\n", res);
	write(log_fd, buf, strlen(buf));


	printf("-----\n");
	res = close(log_fd);
	printf("res:%d\n", res);
	return 0;
}

int f2()
{
	int res = 0;
	int log_fd = 0;
	const char* s = NULL;

	log_fd = open("log", O_RDWR | O_CREAT, 0664);
	res = dup2(log_fd, 1);
	res = close(log_fd);

	// write
	s = "hi-write\n";
	res = write(1, s, strlen(s));
	res = printf("hi-printf\n");
	printf("-----\n");

	return 0;
}

int main()
{
	f2();
}
