#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

void utl1()
{
	char buf[128];
	for(int i = 0; i < 5; i++){
		sprintf(buf, "i:%d\n", i);
		write(STDOUT_FILENO, buf, strlen(buf));
		sleep(1);
	}
	////exit(1);
}

void utl2()
{
	char buf[1024];
	ssize_t count = 0;

	do {
		count = read(STDIN_FILENO, buf, sizeof(buf));
		if (count <= 0) {
			break;
		}
		for(int i = 0; i < count; i++) {
			buf[i] = toupper(buf[i]);
		}
		write(STDOUT_FILENO, buf, count);
	} while(count > 0);
}


int main()
{
	char buf[128];
	pid_t pid;
	int fd[2];

	printf("main:---\n");

	pipe(fd);
	pid = fork();

	if (pid == 0) {
		printf("child1\n");

		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[WRITE_END]);
		utl1();
	} else {
		int status;

		pid = fork();
		if (pid == 0) {
			int retval = 0;
			//int retval = fcntl( fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK);
			printf("child2\n");

			dup2(fd[READ_END], STDIN_FILENO);
			close(fd[READ_END]);
			retval = fcntl( STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
			utl2();
		} else {
			printf("main\n");
			waitpid(pid, &status, 0);
			printf("main:---\n");
		}
	}

	return 0;
}
