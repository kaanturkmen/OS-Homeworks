#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int solveB() {

	// Declearing variables for further use.
	int pid;

	// Creating a child process.
	pid = fork();

	// If pid of the process is smaller than the 0, it means fork has failed.
	if(pid < 0) {
		printf("ERROR: Forking failed.");
		return -1;
	// Pid = 0 means that we are inside of the child process, running ps l command.
	// Please keep in mind that this code is created for the linux operating system.
	} else if (pid == 0) {
		execl("/bin/ps", "/bin/ps", "l", NULL);
	// Pid > 0 means that we are inside of the parent process, making parent wait its child to
	// finish executing its task.
	} else {
		wait(NULL);
		printf("Child finished its process.\n");
	}

	return 0;
}

int main() {
	return solveB();
}
