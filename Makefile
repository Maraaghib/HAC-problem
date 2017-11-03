CC = gcc
CFLAGS = -std=c99
CPPFLAGS =
VPATH =
LDFLAGS =

SOURCES = $(wildcard *.c)

hac2sat : $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: clean
clean :
	-rm -f hac2sat
