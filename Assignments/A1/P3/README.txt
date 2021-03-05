Compile and Run Commands for Problem 3.

Part A:

COMPILE = make
RUN = sudo insmod p3a.ko

Part B:

// Before compiling Part B, please change the Makefile's first line to: obj-m += p3b.o

COMPILE = make
RUN = sudo insmod p3b.ko processID=<pid>

where <pid> is process id of the process.

// In the screenshot, I gave process id 1 which is init process. However, I could
not able to show all siblings in the same picture.
