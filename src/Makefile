CXX = g++
CXXFLAGS = -Wall
LIBS = -lncurses

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

run: main
	./main

clean:
	rm -f main