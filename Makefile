# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -g -O2 -std=c11 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lm
TARGET = battle_city
SRC = battle_city.c

# Regla principal
all: $(TARGET)

# Regla de compilación
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Limpieza
clean:
	rm -f $(TARGET)

.PHONY: all clean
