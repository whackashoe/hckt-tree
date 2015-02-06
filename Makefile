CXXFLAGS=-O2 -std=c++11 -Wall -Wextra -Weffc++

SOURCES= $(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
TARGET=$(lastword $(subst /, ,$(realpath .)))

CXX=g++

all: $(TARGET)
	@echo quadtree_soptim built

$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o quadtree_soptim $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(TARGET)
