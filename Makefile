inc = -lX11
obj = Xmain.o

all: psyterm

psyterm: ${obj}
	${CC} -o $@ ${obj} ${inc}
	setsid -f ./psyterm

%.o: %.c
	${CC} -c ${obj:.o=.c} ${inc}

