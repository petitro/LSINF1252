#
# Makefile for cracker
#
// je ne sais pas comment faire make tout court.
tests:  test.c
	gcc -Wall -Werror tests test.c
all: code.c test.c
	gcc -Wall -Werror -o cracker code.c test.c
clean:
	rm cracker
