cracker : code.o src/reverse.o src/sha256.o
	gcc -Wall -Werror -std=c99 -g -o cracker src/code.c src/sha256.o src/reverse.o -lpthread

code.o : src/code.c src/reverse.h
	gcc -Wall -Werror -std=c99 -c src/code.c

reverse.o : src/reverse.c src/reverse.h src/sha256.h
	gcc -Wall -Werror -std=c99 -c src/reverse.c

sha256.o : src/sha256.c src/sha256.h
	gcc -Wall -Werror -std=c99 -c src/sha256.c

tests: tests/test.c 
	gcc -Wall -Werror -c test tests/tests.c
	./test

all:

clean: 
	rm -rf *.o src/*.o cracker
