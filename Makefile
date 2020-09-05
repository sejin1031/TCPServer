CC=gcc
CFLAGS=-o
TARGETS=server
all: $(TARGETS)
.PHONY: all
%:
	$(CC) -g -o $@ $@.c

clean:
	rm $(TARGETS)
