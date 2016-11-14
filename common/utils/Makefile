#validate_min_cut: validate_min_cut.cpp 
#	g++ -o validate_min_cut -std=c++0x validate_min_cut.cpp

.SUFFIXES = .c .o
CC = g++
CFLAGS = -g -ansi -pedantic-errors -Wall
LDFLAGS = -lpthread -lm

OBJS = common.o push_relabel.o

SOURCES = common.cpp push_relabel.cpp

all: push_relabel common

push_relabel: push_relabel.o
	$(CC) $(CFLAGS) -o $@ push_relabel.cpp

common: common.o
	$(CC) $(CFLAGS) -o $@ common.cpp

clean:
	rm -rf $(OBJS) common push_relabel

