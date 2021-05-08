# To alter the PATH default behavior, INTERN_PATH
# must be set like the line below


#INTERN_PATH=/bin:/usr/bin
INTERN_PATH := $(shell echo $(PATH))

PATH_MACRO= -DBASE_PATH=\"$(INTERN_PATH)\"

CC=gcc

CFLAGS=

CCOPTFLAGS= -O3 -mtune=native

LDFLAGS= -lreadline

CCDBGFLAGS= -Og -ggdb

TARGET=crash

SRCDIR=src
BUILDDIR=build

SOURCES=$(wildcard $(SRCDIR)/*.c)
HEADERS=$(wildcard $(SRCDIR)/*.h)
OBJECTS=$(addprefix $(BUILDDIR)/, $(notdir $(SOURCES:.c=.o)))

.PHONY: clean all debug install

all: $(BUILDDIR) $(TARGET)
	@echo Compilation done!

$(BUILDDIR)/shell.o: $(SRCDIR)/shell.c $(SRCDIR)/shell.h
	$(CC) $(CFLAGS) $(CCOPTFLAGS) $(PATH_MACRO) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.c  $(HEADERS)
	$(CC) $(CFLAGS) $(CCOPTFLAGS) -c -o $@ $<

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

debug: $(SOURCES)
	$(CC) $^ -o $(TARGET) $(CFLAGS) $(CCDBGFLAGS)

clean:
	-rm -r $(TARGET) $(BUILDDIR)

install: $(TARGET)
	install -m 0755 $(TARGET) /usr/bin/$(TARGET)

uninstall:
	rm /usr/bin/$(TARGET)
