#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

/////////////////////////////////////////////////////////////////////////////
// Emulate shell which run 2 commands with pipe, e.g. ls -la | tr 'a-z' 'A-Z'
/////////////////////////////////////////////////////////////////////////////

#define READ_END 0
#define WRITE_END 1

void generate_lines()
{
	char buf[128];
	for(int i = 0; i < 5; i++){
		sprintf(buf, "i:%d\n", i);
		//write(STDOUT_FILENO, buf, strlen(buf));
		printf("%s", buf);
		sleep(1);
	}
}

void transform_lines()
{
	char buf[1024];
	ssize_t count = 0;

	do {
#if 0
		count = read(STDIN_FILENO, buf, sizeof(buf));
		if (count <= 0) {
			break;
		}
#else
		char *p = fgets(buf, sizeof(buf), stdin);
		if (!p) {
			break;
		}
		count = strlen(buf);
#endif
		for(int i = 0; i < count; i++) {
			buf[i] = toupper(buf[i]);
		}
		//write(STDOUT_FILENO, buf, count);
		printf("%s", buf);
	} while(count > 0);
}


int main()
{
	char buf[128];
	pid_t pid;
	int fd[2];

	printf("main-0 start:---\n");

	pipe(fd);
	pid = fork();

	if (pid == 0) {
		printf("child-0 start\n");

		dup2(fd[WRITE_END], STDOUT_FILENO);
		close(fd[READ_END]);
		close(fd[WRITE_END]);
		generate_lines();
		printf("child-0 stop\n");
	} else {

		pid = fork();
		if (pid == 0) {
			int retval = 0;
			//int retval = fcntl( fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK);
			printf("child-1 start\n");

			dup2(fd[READ_END], STDIN_FILENO);
			close(fd[READ_END]);
			close(fd[WRITE_END]);
			//retval = fcntl( STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
			transform_lines();
			printf("child-1 stop\n");
		} else {
			int status;

			close(fd[READ_END]);
			close(fd[WRITE_END]);
			printf("main-1 start\n");
			waitpid(pid, &status, 0);
			printf("main-1 stop:---\n");
		}
	}

	printf("main-0 stop:--- pid:%d\n", pid);
	return 0;
}
