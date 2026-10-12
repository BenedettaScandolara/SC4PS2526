CC = h5cc
CFLAGS = -O2 -Wall

all: hw6

hw6: hw6.c config-parser/parser.h
	$(CC) $(CFLAGS) hw6.c -o hw6 -lm

run: hw6
	mkdir -p output
	./hw6 input.txt

clean:
	rm -f hw6
