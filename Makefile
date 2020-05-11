# To alter the PATH default behavior, INTERN_PATH 
# must be set like the line below 


#INTERN_PATH=/bin:/usr/bin 
INTERN_PATH := $(shell echo $(PATH))

PATH_MACRO= -DBASE_PATH=\"$(INTERN_PATH)\"

CC=gcc
CCFLAGS= -lreadline

CCOPTFLAGS= -O3 -mtune=native
CCDBGFLAGS= -Og -ggdb

TARGET= crash

SOURCES=$(wildcard src/*.c)

.PHONY: clean all debug install

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $^ -o $@ $(CCFLAGS) $(PATH_MACRO) $(CCOPTFLAGS)

debug: $(SOURCES)
	$(CC) $^ -o $(TARGET) $(CCFLAGS) $(PATH_MACRO) $(CCDBGFLAGS)

clean:
	-rm $(TARGET)

install: $(TARGET)
	install -m 0755 $(TARGET) /usr/bin/$(TARGET)

uninstall:
	rm /usr/bin/$(TARGET)
