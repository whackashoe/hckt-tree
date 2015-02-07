#include <iostream>
#include <bitset>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "hckt_tree.hpp"

constexpr size_t window_width  { 512 };
constexpr size_t window_height { 512 };

template <typename Tree>
int render(sf::RenderWindow & window, const Tree & m)
{
    for(size_t a=0; a<4; ++a) {
        for(size_t b=0; b<4; ++b) {
            for(size_t c=0; c<4; ++c) {
                if(m.empty()) {
                    continue;
                }

                auto pos = Tree::get_position_2d(a, b, c);
                auto value = m.get_value(pos);
                auto x = Tree::get_x_2d(a, b, c);
                auto y = Tree::get_y_2d(a, b, c);
                sf::Uint8 col = value * 20;

                sf::RectangleShape square { sf::Vector2f { 64, 64 } };
                square.setFillColor(sf::Color { col, col, col, 255 } );
                square.setPosition(64*x, 64*y);
                window.draw(square);

                for(size_t a2=0; a2<4; ++a2) {
                    for(size_t b2=0; b2<4; ++b2) {
                        for(size_t c2=0; c2<4; ++c2) {
                            if(m.leaf(pos)->empty()) {
                                continue;
                            }
                            auto subpos = Tree::get_position_2d(a2, b2, c2);
                            auto subvalue = m.leaf(pos)->get_value(subpos);
                            auto subx = Tree::get_x_2d(a2, b2, c2);
                            auto suby = Tree::get_y_2d(a2, b2, c2);
                            sf::Uint8 col = subvalue * 20;

                            sf::RectangleShape square { sf::Vector2f { 8, 8 } };
                            square.setFillColor(sf::Color { col, col, col, 100 } );
                            square.setPosition((64 * x) + (8 * subx), (64 * y) + (8 * suby));
                            window.draw(square);



                            for(size_t a3=0; a3<4; ++a3) {
                                for(size_t b3=0; b3<4; ++b3) {
                                    for(size_t c3=0; c3<4; ++c3) {
                                        if(m.leaf(pos)->leaf(subpos)->empty()) {
                                            continue;
                                        }
                                        auto subsubpos = Tree::get_position_2d(a3, b3, c3);
                                        auto subsubvalue = m.leaf(pos)->leaf(subpos)->get_value(subsubpos);

                                        auto subsubx = Tree::get_x_2d(a3, b3, c3);
                                        auto subsuby = Tree::get_y_2d(a3, b3, c3);
                                        sf::Uint8 col = subsubvalue * 20;

                                        sf::RectangleShape square { sf::Vector2f { 1, 1 } };
                                        square.setFillColor(sf::Color { col, col, col, 100 } );
                                        square.setPosition((64 * x) + (8 * subx) + (1 * subsubx), (64 * y) + (8 * suby) + (1 * subsuby));
                                        window.draw(square);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    while(1) {
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
void populate(Tree & m)
{
    for(size_t a=0; a<4; ++a) {
        for(size_t b=0; b<4; ++b) {
            for(size_t c=0; c<4; ++c) {
                auto pos = Tree::get_position_2d(a, b, c);
                m.insert(pos, a+b+c);

                if(pos % 6 == 0) { 
                    for(size_t a2=0; a2<4; ++a2) {
                        for(size_t b2=0; b2<4; ++b2) {
                            for(size_t c2=0; c2<4; ++c2) {
                                auto subpos = Tree::get_position_2d(a2, b2, c2);
                                m.leaf(pos)->insert(subpos, a2+b2+c2);

                                if(subpos % 6 == 0) { 
                                    for(size_t a3=0; a3<4; ++a3) {
                                        for(size_t b3=0; b3<4; ++b3) {
                                            for(size_t c3=0; c3<4; ++c3) {
                                                auto subsubpos = Tree::get_position_2d(a3, b3, c3);
                                                m.leaf(pos)->leaf(subpos)->insert(subsubpos, a3+b3+c3);


                                                if(subsubpos % 6 == 0) { 
                                                    for(size_t a4=0; a4<4; ++a4) {
                                                        for(size_t b4=0; b4<4; ++b4) {
                                                            for(size_t c4=0; c4<4; ++c4) {
                                                                auto subsubsubpos = Tree::get_position_2d(a4, b4, c4);
                                                                m.leaf(pos)->leaf(subpos)->leaf(subsubpos)->insert(subsubsubpos, a4+b4+c4);

                                                                if(subsubsubpos % 6 == 0) { 
                                                                    for(size_t a5=0; a5<4; ++a5) {
                                                                        for(size_t b5=0; b5<4; ++b5) {
                                                                            for(size_t c5=0; c5<4; ++c5) {
                                                                                auto subsubsubsubpos = Tree::get_position_2d(a5, b5, c5);
                                                                                m.leaf(pos)->leaf(subpos)->leaf(subsubpos)->leaf(subsubsubpos)->insert(subsubsubsubpos, a5+b5+c5);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


int main(int argc, char ** argv)
{
    hckt_tree<int> m;
    populate(m);    
    std::cout << m.calculate_memory_size() << std::endl;

    sf::RenderWindow window{{window_width, window_height}, "hckt-tree"};
    window.clear(sf::Color { 255, 255, 255, 255 } );

    return render(window, m);
}
