CC = gcc
CFLAGS = -std=c99
CPPFLAGS = -D_XOPEN_SOURCE=700
VPATH =
LDFLAGS =

SOURCES = $(wildcard *.c)
SOURCES_L = $(wildcard *.l)

EXE = hac2sat vars2graph

all: $(EXE)

hac2sat : hac2sat.c $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@


vars2graph: lex.yy.o
	rm -f lex.yy.c # Pour ne pas le tenir en compte à la génération de l'exécutable hac2sat
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

lex.yy.o: lex.yy.c

lex.yy.c: $(SOURCES_L)
	lex $<


.PHONY: clean
clean :
	-rm -f $(EXE) *.o
