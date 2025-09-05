CFLAGS=-Wall -Wextra -pedantic

all: ff.c.o ff.cxx.o

ff.cxx.o: src/include/ff.h
	$(CC) $(CFLAGS) -DLA_IMPLEMENTATION -x c++ -o ff.cxx.o -c src/include/ff.h

ff.c.o: src/include/ff.h
	$(CC) $(CFLAGS) -DLA_IMPLEMENTATION -std=c99 -x c -o ff.c.o -c src/include/ff.h

ff: src/main.c
	$(CC) $(CFLAGS) -std=c17 -ggdb -o ff src/main.c

clean:
	rm -f ff ff.c.o ff.cxx.o src/include/ff.h

.PHONY: all clean
