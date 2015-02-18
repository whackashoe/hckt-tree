#include <hckt/tree.hpp>
#include <hckt/bench.hpp>
#include <hckt/lmemvector.hpp>

constexpr size_t window_width  { 512 };
constexpr size_t window_height { 512 };

#include "inc_populate_2d_sparse.cpp"
#include "inc_render_2d_a.cpp"



int main()
{
    hckt::tree<uint32_t> m;
    populate(m, 16006);
    hckt::bench::mem_usage_info(&m);

    sf::RenderWindow window{{window_width, window_height}, "hckt-tree"};
    window.clear(sf::Color { 255, 255, 255, 255 } );

    return zoom_render(window, m, 1.06);
}
