#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int solveC() {

	// Creating variables for further use.
	int pid;

	// Creating a child process.
	pid = fork();

	// If pid of the process is smaller than the 0, it means fork has failed.
	if(pid < 0) {
		printf("ERROR: Forking failed.");
		return -1;
	// Pid = 0 means that we are inside of the child process, terminating the child process.
	} else if (pid == 0) {
		exit(0);
	// Pid > 0 means that we are inside of the parent process, forcing parent
	// process to sleep for observing the zombie child process.
	} else {
		sleep(5);
	}

	return 0;
}

int main() {
	return solveC();
}
