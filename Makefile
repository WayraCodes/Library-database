CC = gcc

CFLAGS = -Wall

SRC = library_main.c borrow.c catalogue.c file_manager.c

OBJ = $(SRC:.c=.o)

TARGET = library

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
