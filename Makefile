# Settings
CXX=g++
CXXFLAGS=-Wall -pedantic -g -std=c++11
CC=$(CXX)

# Programs
ALL_T=word_graph check gen
all: $(ALL_T)
word_graph: word_graph.o properties.o graph.o bfs.o
check: check.o properties.o graph.o bfs.o
gen: gen.o special.o graph.o

# Dependencies
check.cpp: graph.h properties.h
special.cpp: graph.h special.h
properties.cpp: graph.h properties.h bfs.h
word_graph.cpp: graph.h properties.h bfs.h
graph.cpp: graph.h
bfs.cpp: graph.h bfs.h

clean:
	rm -f $(ALL_T) *.o *.stackdump
