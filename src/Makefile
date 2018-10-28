CC=gcc
LIBS=-lGL -lcurl -lopenal -lpthread -lX11 -lm
CFLAGS=-g -w -no-pie -rdynamic
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
COMMITSHA=$(shell git rev-parse HEAD | cut -c '1-7')

EXECUTABLE=ClassiCube

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -DCC_COMMIT_SHA=\"$(COMMITSHA)\" -c $< $(LIBS) -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)