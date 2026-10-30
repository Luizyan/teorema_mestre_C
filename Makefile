CC=gcc
CFLAGS=-O2 -std=gnu11 -Wall -Wextra
all: p1_matmul p2_mergesort3 p3_inversoes
p1_matmul: p1_matmul.c util.h
	$(CC) $(CFLAGS) -o $@ $< -lm
p2_mergesort3: p2_mergesort3.c util.h
	$(CC) $(CFLAGS) -o $@ $<
p3_inversoes: p3_inversoes.c util.h
	$(CC) $(CFLAGS) -o $@ $<
run: all
	rm -f resultados.csv
	./p1_matmul resultados.csv 512
	./p2_mergesort3 resultados.csv 14
	./p3_inversoes resultados.csv 16
clean:
	rm -f p1_matmul p2_mergesort3 p3_inversoes
