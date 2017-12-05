#include "stdio.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <err.h>

int gMyTraceFd = 0;
char gBuf[128];
/*
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
*/
int redirect2null()
{
	int res = 0;
	int null_fd = 0;
	const char* s = NULL;

	null_fd = open("/dev/null", O_RDWR);
	sprintf(gBuf, "null_fd:%d\n", null_fd);		write(gMyTraceFd, gBuf, strlen(gBuf));

	res = dup2(null_fd, STDOUT_FILENO);
	sprintf(gBuf, "dup2 res:%d\n", res);		write(gMyTraceFd, gBuf, strlen(gBuf));

	// write
	s = "hi-write\n";
	res = write(1, s, strlen(s));
	sprintf(gBuf, "write res:%d\n", res);		write(gMyTraceFd, gBuf, strlen(gBuf));
	res = printf("hi-printf\n");
	sprintf(gBuf, "printf res:%d\n", res);		write(gMyTraceFd, gBuf, strlen(gBuf));
	printf("-----\n");

	return 0;
}

int redirect2file()
{
	int res = 0;
	int file_fd = 0;
	const char* s = NULL;

	file_fd = open("myfile", O_RDWR | O_CREAT, 0664);
	sprintf(gBuf, "file_fd:%d\n", file_fd);		write(gMyTraceFd, gBuf, strlen(gBuf));

	res = dup2(file_fd, STDOUT_FILENO);
	sprintf(gBuf, "dup2 res:%d\n", res);		write(gMyTraceFd, gBuf, strlen(gBuf));

	res = close(file_fd);
	sprintf(gBuf, "close res:%d\n", res);		write(gMyTraceFd, gBuf, strlen(gBuf));

	// write
	s = "hi-write\n";
	res = write(STDOUT_FILENO, s, strlen(s));
	sprintf(gBuf, "write res:%d\n", res);		write(gMyTraceFd, gBuf, strlen(gBuf));
	res = printf("hi-printf\n");
	sprintf(gBuf, "printf res:%d\n", res);		write(gMyTraceFd, gBuf, strlen(gBuf));
	printf("-----\n");

	return 0;
}

int main()
{
	gMyTraceFd = open("mytrace", O_RDWR | O_CREAT | O_TRUNC, 0664);
//	redirect2null();
	redirect2file();
}
