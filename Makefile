CXXFLAGS=-O2 -std=c++11 -Wall -Wextra -Weffc++
LDFLAGS = -lsfml-system -lsfml-graphics -lsfml-window
CXX=g++

all: install examples

install:
	sudo mkdir -p /usr/local/include/hckt
	sudo cp include/*.hpp /usr/local/include/hckt

examples: 2drender 2drender_lowmem
	@echo examples built

2drender: examples/2drender.cpp
	@$(CXX) $(CXXFLAGS) -o examples/2drender examples/2drender.cpp $(LDFLAGS)

2drender_lowmem: examples/2drender_lowmem.cpp
	@$(CXX) $(CXXFLAGS) -o examples/2drender_lowmem examples/2drender_lowmem.cpp $(LDFLAGS)

clean:
	rm examples/2drender examples/2drender_lowmem
