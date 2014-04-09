#TODO CFLAGS doesn't do anything
CC=cc
CFLAGS=-Wall

build: bin_dir build/shell.o build/lex.yy.o
	$(CC) -o bin/sish build/shell.o build/lex.yy.o


build/shell.o: build_dir src/shell.c
	$(CC) -o build/shell.o -c src/shell.c

build/lex.yy.o: build/lex.yy.c
	$(CC) -o build/lex.yy.o -c build/lex.yy.c

build/lex.yy.c: build_dir src/lex.c
	flex -obuild/lex.yy.c src/lex.c

build_dir:
	@mkdir -p build

bin_dir:
	@mkdir -p bin

spotless: clean
	rm -rf bin

clean:
	rm -rf build



