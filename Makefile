CXXFLAGS=-O2 -std=c++11 -Wall -Wextra -Weffc++
LDFLAGS = -lsfml-system -lsfml-graphics -lsfml-window
CXX=g++

all: install examples

install:
	sudo mkdir -p /usr/local/include/hckt
	sudo cp include/*.hpp /usr/local/include/hckt

examples: 2d
	@echo examples built

2d: examples/2d.cpp
	@$(CXX) $(CXXFLAGS) -o examples/2d examples/2d.cpp $(LDFLAGS)

clean:
	rm examples/2d
