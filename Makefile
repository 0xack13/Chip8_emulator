CFLAGS=-pedantic -Wall -Wextra -Werror -Wfatal-errors -Ofast -flto -march=native -pipe
LIBS=-lSDL2
SRC=src/main.c src/chip8.c
CC=gcc

emu: $(SRC)
	$(CC) $(CFLAGS) -o emu $(SRC) $(LIBS)