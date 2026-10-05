CC = gcc
CFLAGS = -O3 -std=c11 -Wall -Wextra -Wpedantic
OMPFLAGS = -fopenmp

all: build/merge_sort_serial build/merge_sort_paralelo

build:
	mkdir -p build

build/merge_sort_serial: merge_sort_serial.c | build
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@

build/merge_sort_paralelo: merge_sort_paralelo.c | build
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@

clean:
	rm -f build/merge_sort_serial build/merge_sort_paralelo

.PHONY: all clean
