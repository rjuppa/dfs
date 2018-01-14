CC=gcc
CFLAGS=-std=c99 -Wall

all: dfs

dfs: main.c graph.c stack.c
	gcc -o dfs.exe -std=c11 -Wall main.c graph.c stack.c

clean:
	rm -f dfs.exe
