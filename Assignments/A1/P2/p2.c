#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// Defining macros.
#define BUFFER_SIZE 50
#define READ_END 0
#define WRITE_END 1

int solve() {

	// Creating buffers for read and write operations.
	char timeAwrite[BUFFER_SIZE];
	char timeAread[BUFFER_SIZE];

	char timeBwrite[BUFFER_SIZE];
	char timeBread[BUFFER_SIZE];

	char timeCwrite[BUFFER_SIZE];
	char timeCread[BUFFER_SIZE];

	char lastTime[BUFFER_SIZE];

	// Defining int arrays to use for piping.
	int fdAtoB[2];
	int fdBtoC[2];
	int fdCtoB[2];
	int fdBtoA[2];

	// Defining pid variables for further use.
	pid_t pidB;
	pid_t pidC;

	// Creating timeval struct and time_t variable.
	struct timeval tv;
	time_t currtime;

	// Creating pipes.
	if (pipe(fdAtoB) == -1) {
		fprintf(stderr, "ERROR: Piping failed. (A -> B)");
		return 1;
	}

	if (pipe(fdBtoA) == -1) {
		fprintf(stderr, "ERROR: Piping failed. (B -> A)");
		return 1;
	}

	// Important note: Creating pipes before forking processes prevents any
	// accesing issue.

	// Creating the child process B.
	pidB = fork();

	// If pid of the process is smaller than the 0, it means fork has failed.
	if (pidB < 0) {
		fprintf(stderr, "ERROR: Fork failed. (Process B)");
		return -1;
	}

	// Pid > 0 means that we are inside of the parent process, getting current
	// time, formatting it and writing it into a pipe.
	if (pidB > 0) {
		gettimeofday(&tv, NULL);
		currtime = tv.tv_sec;
		strftime(timeAwrite, BUFFER_SIZE, "%m-%d-%Y  %T.", localtime(&currtime));

		close(fdAtoB[READ_END]);
		write(fdAtoB[WRITE_END], timeAwrite, strlen(timeAwrite) + 1);
		close(fdAtoB[WRITE_END]);

		// Waiting signal from child process B to continue.
		wait(NULL);

		// Reading the incoming data from process B. (which is came to B from C.)
		close(fdBtoA[WRITE_END]);
		read(fdBtoA[READ_END], lastTime, BUFFER_SIZE);
		printf("A read from B (Data from C.): %s\n", lastTime);
		close(fdBtoA[READ_END]);

		// Killing process B.
		kill(pidB, SIGKILL);

	// Else we are inside a child process, reading time from pipe, which is arrived
	// from its parent process.
	} else {
		close(fdAtoB[WRITE_END]);
		read(fdAtoB[READ_END], timeAread, BUFFER_SIZE);
		printf("B read from A: %s\n", timeAread);
		close(fdAtoB[READ_END]);

		// Sleeps for 3 seconds.
		sleep(3);

		// Creating pipes for B -> C and C -> B communication.
		if (pipe(fdBtoC) == -1) {
			fprintf(stderr, "ERROR: Pipe failed. (B -> C)");
			return 1;
		}

		if (pipe(fdCtoB) == -1) {
			fprintf(stderr, "ERROR: Pipe failed. (B -> C)");
			return 1;
		}

		// Creating process C.
		pidC = fork();

		if (pidC < 0) {
			fprintf(stderr, "ERROR: Fork failed. (Process C)");
			return 1;
		}

		// Pid > 0 means that we are inside of the parent process, getting current
		// time, formatting it and writing it into a pipe.
		if (pidC > 0) {
			gettimeofday(&tv, NULL);
			currtime = tv.tv_sec;
			strftime(timeBwrite, BUFFER_SIZE, "%m-%d-%Y  %T.", localtime(&currtime));

			close(fdBtoC[READ_END]);
			write(fdBtoC[WRITE_END], timeBwrite, strlen(timeBwrite) + 1);
			close(fdBtoC[WRITE_END]);

			// Waiting signal from child process C.
			wait(NULL);

			// Reading data from its child process.
			close(fdCtoB[WRITE_END]);
			read(fdCtoB[READ_END], timeCread, BUFFER_SIZE);
			printf("B read from C: %s\n", timeCread);
			close(fdCtoB[READ_END]);

			// Sleeps for 3 seconds.
			sleep(3);

			// Writing the time which is read from C to the pipe between B -> A.
			close(fdBtoA[READ_END]);
			write(fdBtoA[WRITE_END], timeCread, strlen(timeCread) + 1);
			close(fdBtoA[WRITE_END]);

			// Killing process C.
			kill(pidC, SIGKILL);

		// Else we are inside a child process, reading time from pipe, which is arrived
		// from its parent process.
		} else {
			close(fdBtoC[WRITE_END]);
			read(fdBtoC[READ_END], timeBread, BUFFER_SIZE);
			printf("C read from B: %s\n", timeBread);
			close(fdBtoC[READ_END]);

			// Sleeps for 3 seconds.
			sleep(3);

			// Getting current time and passing it to its parent process B.
			gettimeofday(&tv, NULL);
			currtime = tv.tv_sec;
			strftime(timeCwrite, BUFFER_SIZE, "%m-%d-%Y  %T.", localtime(&currtime));

			close(fdCtoB[READ_END]);
			write(fdCtoB[WRITE_END], timeCwrite, strlen(timeCwrite) + 1);
			close(fdCtoB[WRITE_END]);
		}
	}

	return 0;
}

int main() {
	return solve();
}
