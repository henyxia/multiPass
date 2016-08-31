#FILE	: Makefile
#Author	: Jean "henyxia" Wasilewski	

CC=gcc
CFLAGS=-c -Wall -std=c99 -g
SOURCES=main.c ui.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=multiPass

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	rm $(OBJECTS) $(EXECUTABLE)
