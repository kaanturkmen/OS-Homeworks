#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int solveA() {
	// Declearing variables for further use.
	int level = 0;
	int pid;

	// Printing base processes' properties.
	printf("\n\tBase Process ID: %d, level: %d\n", getpid(), level);

	// Incrementing level variable.
	level++;

	// Looping 4 times.
	for (int i = 0; i < 4; i++) {

		// Creating a child process.
		pid = fork();

		// If pid of the process is smaller than the 0, it means fork has failed.
		if (pid < 0) {
			printf("ERROR: Forking failed.");
			return -1;
		// Pid = 0 means that we are inside of the child process, printing required outputs.
		} else if (pid == 0) {
			printf("Process ID: %d, Parent ID: %d, level: %d\n", getpid(), getppid(), level);
			level++;
		// Pid > 0 means that we are inside of the parent process, making parent wait its child to
		// finish executing its task.
		} else {
			wait(NULL);
		}
	}

	return 0;
}

int main() {
	return solveA();
}
