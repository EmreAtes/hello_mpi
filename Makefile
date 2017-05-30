CC = mpicc

all: hello.out

hello.out: hello.c
	$(CC)  -o hello.out hello.c

copy: hello.c
	$(CC) -o hello.out.copy hello.c

clean:
	rm -rf hello *.o
