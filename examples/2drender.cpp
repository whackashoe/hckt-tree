#include <iostream>
#include <bitset>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <hckt/tree.hpp>
#include <hckt/lmemvector.hpp>

constexpr size_t window_width  { 512 };
constexpr size_t window_height { 512 };


template <typename Tree>
void recursive_render(sf::RenderWindow & window, Tree * m, const float rsize, const float offset_x, const float offset_y)
{
    if(offset_x > window_width || offset_y > window_height) {
        return;
    }

    if(m->empty()) {
        return;
    }

    if(rsize < 1.0f / 8.0f) {
        return;
    }

    for(size_t a=0; a<4; ++a) {
        for(size_t b=0; b<4; ++b) {
            for(size_t c=0; c<4; ++c) {
                const auto pos   = Tree::get_position_2d(a, b, c);
                const auto ox    = offset_x + (rsize * Tree::get_x_2d(a, b, c));
                const auto oy    = offset_y + (rsize * Tree::get_y_2d(a, b, c));
                const sf::Uint8 col = m->get_value(pos) * (255 / (4 * 4 * 4));

                sf::RectangleShape square { sf::Vector2f { rsize, rsize } };
                square.setFillColor(sf::Color { col, col, col, 255 } );
                square.setPosition(ox, oy);
                window.draw(square);

                recursive_render(window, m->leaf(pos), rsize / 8.0f, ox, oy);
            }
        }
    }
}

template <typename Tree>
int render(sf::RenderWindow & window, const Tree & m)
{
    double rsize = 64;

    while(1) {
        recursive_render(window, &m, rsize, 0, 0);
        rsize *= 1.01;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            return 0;
        }

        window.display();
    }

    return 0;
}


template <typename Tree>
void recursive_populate(Tree * m, const int depth, const int max_depth)
{
    if(depth >= max_depth) {
        return;
    }

    for(size_t a=0; a<4; ++a) {
        for(size_t b=0; b<4; ++b) {
            for(size_t c=0; c<4; ++c) {
                const auto pos = Tree::get_position_2d(a, b, c);
                const auto alg = (1 + a) * (1 + b) * (1 + c);
                m->insert(pos, alg);

                if(alg % 5 == 1) { 
                    recursive_populate(m->leaf(pos), depth + 1, max_depth);
                }
            }
        }
    }
}

template <typename Tree>
void populate(Tree & m, const int depth)
{
    recursive_populate(&m, 0, depth);
}

int main(int argc, char ** argv)
{
    hckt_tree<uint32_t> m;
    populate(m, 6);
    std::cout << m.calculate_memory_size() << std::endl;

    sf::RenderWindow window{{window_width, window_height}, "hckt-tree"};
    window.clear(sf::Color { 255, 255, 255, 255 } );

    return render(window, m);
}
