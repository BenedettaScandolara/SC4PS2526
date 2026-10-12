CC = gcc
CFLAGS = -O2 -Wall

all: run

legendre: legendre.c
	$(CC) $(CFLAGS) legendre.c -o legendre -lm

run: legendre
	mkdir -p output
	./legendre
	python3 reference_and_plots.py

clean:
	rm -rf legendre output
