#include <iostream>
#include <bitset>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <hckt/tree.hpp>
#include <hckt/lmemvector.hpp>

constexpr size_t window_width  { 512 };
constexpr size_t window_height { 512 };

#include "inc_populate_2d_sparse.cpp"
#include "inc_render_2d_a.cpp"



int main()
{
    hckt::tree<uint32_t> m;
    populate(m, 16006);
    m.mem_usage_info();

    sf::RenderWindow window{{window_width, window_height}, "hckt-tree"};
    window.clear(sf::Color { 255, 255, 255, 255 } );

    return zoom_render(window, m, 1.06);
}
