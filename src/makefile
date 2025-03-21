CC = gcc
CFLAGS = -Wall -g
LDFLAGS = `sdl2-config --cflags --libs`

SRC = chip8.c interface.c main.c  
OBJ = $(SRC:.c=.o)
EXEC = chip8_emulator

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

chip8.o: chip8.c chip8.h
	$(CC) $(CFLAGS) -c chip8.c

interface.o: interface.c interface.h
	$(CC) $(CFLAGS) -c interface.c

main.o: main.c chip8.h
	$(CC) $(CFLAGS) -c main.c  # Added rule for main.c

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ROM)

.PHONY: all clean run