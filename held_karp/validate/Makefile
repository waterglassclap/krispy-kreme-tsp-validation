#validate_min_cut: validate_min_cut.cpp 
#	g++ -o validate_min_cut -std=c++0x validate_min_cut.cpp

.SUFFIXES = .c .o
CC = g++
CFLAGS = -g -ansi -pedantic-errors -Wall
LDFLAGS = -lpthread -lm

UTIL_DIR = ../../utils

OBJS = sample.o

SOURCES = $(UTIL_DIR)/common_util.cpp $(UTIL_DIR)/push_relabel.cpp sample.cpp

all: sample

sample: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

clean:
	rm -rf $(OBJS) sample sample.dSYM

