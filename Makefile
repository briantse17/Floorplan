CXX=g++
CXXFLAGS=-g -Wall -std=c++11
DEFS=-DDEBUG

OBJS=floorplan.o

all: floorplan

floorplan: $(OBJS)
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ $(OBJS) 

floorplan.o: floorplan.cpp bst.h avlbst.h print_bst.h
	$(CXX) $(CXXFLAGS) $(DEFS) -o $@ -c floorplan.cpp 


clean:
	rm -f *.o puzzle
