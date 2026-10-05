CC = clang
CFLAGS = -O3 -std=c11 -Wall -Wextra -Wpedantic
UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
OMP_PREFIX ?= /opt/homebrew/opt/libomp
OMPFLAGS = -Xpreprocessor -fopenmp -isystem $(OMP_PREFIX)/include
OMPLIBS = -L$(OMP_PREFIX)/lib -Wl,-rpath,$(OMP_PREFIX)/lib -lomp
else
OMPFLAGS = -fopenmp
OMPLIBS = -fopenmp
endif

all: build/merge_sort_serial build/merge_sort_paralelo

build:
	mkdir -p build

build/merge_sort_serial: merge_sort_serial.c | build
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@ $(OMPLIBS)

build/merge_sort_paralelo: merge_sort_paralelo.c | build
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@ $(OMPLIBS)

.PHONY: all
