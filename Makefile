#FILE	: Makefile
#Author	: Jean "henyxia" Wasilewski	

CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -std=c99 -g -DDEBUG -D_XOPEN_SOURCE
LIBS=-lpthread
SOURCES=main.c common.c ui.c config.c input.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=multiPass

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	rm -f $(OBJECTS) $(EXECUTABLE)
