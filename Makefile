CC = gcc
CFLAGS = -Wall -Wextra -O2 -fcommon
LDFLAGS = -lX11

SRC_DIR = ./src
BUILD_DIR = ./build

TARGET = psyterm

OBJS = $(addprefix $(BUILD_DIR)/, Xmain.o ptymain.o)

all: always $(TARGET)

always:

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
