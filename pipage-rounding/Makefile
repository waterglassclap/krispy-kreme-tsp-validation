.SUFFIXES = .c .o
CC = g++
CFLAGS = -g -ansi -pedantic-errors -Wall
LDFLAGS = -lpthread -lm

UTIL_DIR = ../../utils

OBJS = randomized.o

SOURCES = $(UTIL_DIR)/common_util.cpp $(UTIL_DIR)/push_relabel.cpp abs_ppr.cpp randomized_ppr.cpp

all: randomized

randomized: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

clean:
	rm -rf $(OBJS) randomized

