CXXFLAGS=-O2 -std=c++11 -Wall -Wextra -Weffc++
LDFLAGS = -lsfml-system -lsfml-graphics -lsfml-window

SOURCES= $(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
TARGET=$(lastword $(subst /, ,$(realpath .)))

CXX=g++

all: $(TARGET)
	@echo hckt-tree built

$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o hckt-tree $(OBJECTS) $(LDFLAGS)

clean:
	rm -rf $(OBJECTS) $(TARGET)
