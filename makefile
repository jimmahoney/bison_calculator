# Makefile

## This example apparently wants a more recent version of bison (3.0.4)
## than I have in the system (2.3). For example, the %code directive
## is not part of 2.3 . So I've downloaded and manually compiled bison3.0.4
## and added an alias for it below.

FILES	= lexer.c parser.c expression.c main.c
CC	= g++
CFLAGS	= -g -x c -ansi

## a manually installed version of bison
BISON3  = /usr/local/bin/bison

test:		$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o test

lexer.c:	lexer.l 
		flex lexer.l

parser.c:	parser.y lexer.c
		$(BISON3) parser.y

clean:
		rm -f *.o *~ lexer.c lexer.h parser.c parser.h test
