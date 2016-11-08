#validate_min_cut: validate_min_cut.cpp 
#	g++ -o validate_min_cut -std=c++0x validate_min_cut.cpp

.SUFFIXES = .c .o
CC = g++
CFLAGS = -g -ansi -pedantic-errors -Wall
LDFLAGS = -lpthread -lm

OBJS = utils.o push_relabel.o sample.o

SOURCES = utils.cpp push_relabel.cpp sample.cpp

all: sample

sample: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

clean:
	rm -rf $(OBJS) utils push_relabel sample

