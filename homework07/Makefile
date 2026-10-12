CC = gcc
CFLAGS = -O2 -Wall $(shell gsl-config --cflags)
LIBS = $(shell gsl-config --libs) -lm

all: run

hw7: hw7.c
	$(CC) $(CFLAGS) hw7.c -o hw7 $(LIBS)

run: hw7
	mkdir -p output
	./hw7 | tee output/run_log.txt
	python3 plots.py | tee -a output/run_log.txt

clean:
	rm -rf hw7 output
