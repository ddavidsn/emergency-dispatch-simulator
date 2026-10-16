CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = tema1
SRCS = main.c functions.c structures.c queuesnstacks.c
HDRS = functions.h structures.h queuesnstacks.h

build: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) tema1.out

.PHONY: build run clean