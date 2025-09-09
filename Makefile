# Compiler
CC = gcc

# Compiler & linker flags
CFLAGS = -Wall \
    -I C:/msys64/mingw64/include/ncurses \
    -I C:/msys64/mingw64/include/SDL2 \
    -DNCURSES_STATIC
LDFLAGS = -L C:/msys64/mingw64/lib -lncurses -lSDL2 -lSDL2_mixer

# Sources and target
SRC = main.c tetris_mech.c
OBJ = $(SRC:.c=.o)
TARGET = TETRIS_REMIX.exe

# Default rule
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c tetris_mech.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del $(OBJ) $(TARGET) 2>nul || true