# Compiler
CC = gcc
# Program files
FILES= main.c
# Other flags
FLAGS=
# Output flags
OFLAGS= -o
# Out file
OUTF=protsh
# Debug flags
DBGFLAGS= -D DEBUG=1
# Debug files
DBGF=$(FILES)

all:
	$(CC) $(FILES) $(OFLAGS) $(OUTF) 
debug:
	$(CC) $(DBGF) $(DBGFLAGS) $(OFLAGS) $(OUTF)
install:
	install -m 0755 $(OUTF) /usr/bin/$(OUTF)
clean:
	rm $(OUTF)
