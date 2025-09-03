inc = -lX11 -l/usr/include/freetype2
obj = Xmain.o

all: psyterm

psyterm: ${obj}
	${CC} -o $@ ${obj} ${inc}

%.o: %.c
	${CC} -c ${obj:.o=.c} ${inc}

