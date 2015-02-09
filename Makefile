CXXFLAGS=-O2 -std=c++11 -Wall -Wextra -Weffc++
LDFLAGS = -lsfml-system -lsfml-graphics -lsfml-window
CXX=g++

all: install examples

install:
	sudo mkdir -p /usr/local/include/hckt
	sudo cp include/*.hpp /usr/local/include/hckt

examples: 2d_zoom_render 2d_zoom_render_lowmem 2d_zoom_render_deep_sparse
	@echo examples built

2d_zoom_render: examples/2d_zoom_render.cpp
	@$(CXX) $(CXXFLAGS) -o examples/2d_zoom_render examples/2d_zoom_render.cpp $(LDFLAGS)

2d_zoom_render_lowmem: examples/2d_zoom_render_lowmem.cpp
	@$(CXX) $(CXXFLAGS) -o examples/2d_zoom_render_lowmem examples/2d_zoom_render_lowmem.cpp $(LDFLAGS)

2d_zoom_render_deep_sparse: examples/2d_zoom_render_deep_sparse.cpp
	@$(CXX) $(CXXFLAGS) -o examples/2d_zoom_render_deep_sparse examples/2d_zoom_render_deep_sparse.cpp $(LDFLAGS)

clean:
	rm examples/2d_zoom_render examples/2d_zoom_render_lowmem
