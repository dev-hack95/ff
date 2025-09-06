CFLAGS=-Wall -Wextra -pedantic

all: ff.c.o ff.cxx.o ff

debug_all: ff.c.o ff.cxx.o debug

ff.cxx.o: src/include/ff.h
	$(CC) $(CFLAGS)  -x c++ -o ff.cxx.o -c src/include/ff.h

ff.c.o: src/include/ff.h
	$(CC) $(CFLAGS) -std=c99 -x c -o ff.c.o -c src/include/ff.h

ff: src/main.c
	$(CC) $(CFLAGS) -std=c17 -ggdb -o ff src/main.c

debug: src/main.c
	$(CC) $(CFLAGS) -std=c17 -ggdb -o ff src/main.c -g


clean:
	rm -f ff ff.c.o ff.cxx.o

.PHONY: all clean
