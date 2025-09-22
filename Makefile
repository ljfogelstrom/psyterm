CC = gcc
CFLAGS = -Wall -Wextra -O2 -fcommon
LDFLAGS = -lX11

SRC_DIR = ./src
BUILD_DIR = ./build

TARGET = psyterm

OBJS = $(addprefix ${BUILD_DIR}/, Xmain.o ptymain.o)

all: always ${TARGET}

${TARGET}: ${OBJS}
	${CC} ${CFLAGS} -o $@ $^ ${LDFLAGS}

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c
	${CC} ${CFLAGS} -c $< -o $@ ${LDFLAGS}

always:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR} ${TARGET}
