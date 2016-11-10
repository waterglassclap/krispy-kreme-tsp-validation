.SUFFIXES = .c .o
CC = g++
CFLAGS = -g -ansi -pedantic-errors -Wall
LDFLAGS = -lpthread -lm

UTIL_DIR = ../utils

OBJS = sample.o

SOURCES = $(UTIL_DIR)/common_util.cpp $(UTIL_DIR)/push_relabel.cpp abs_ppr.cpp randomized_ppr.cpp sample.cpp

all: sample

sample: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

clean:
	rm -rf $(OBJS) sample sample.dSYM

