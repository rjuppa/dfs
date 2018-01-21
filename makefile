CC=gcc
CFLAGS=-std=c99 -Wall

all: dfs

dfs: main.c graph.c stack.c parse_date.c
	gcc -o dfs.exe -std=c11 -Wall main.c graph.c stack.c parse_date.c

clean:
	rm -f dfs.exe
