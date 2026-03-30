CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = compte_est_bon
SRC = compte_est_bon.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
