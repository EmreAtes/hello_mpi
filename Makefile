CC := cc
SRCS := hello.c

OBJS := ${SRCS:.c=.o}
PROGS := ${SRCS:.c=.out}

.PHONY: all
all: ${PROGS}

${PROGS}: ${OBJS}
	${CC} $< -o $@

%.o: %.c
	${CC} -c $<

clean:
	rm -rf ${PROGS} ${OBJS}
