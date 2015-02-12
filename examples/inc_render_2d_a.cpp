template <typename Tree>
void recursive_render(sf::RenderWindow & window, Tree * m, const double rsize, const float offset_x, const float offset_y)
{
    if(offset_x > window_width || offset_y > window_height) {
        return;
    }

    if(m->empty()) {
        return;
    }

    if(rsize < 1) {
        return;
    }

    for(size_t a=0; a<4; ++a) {
        for(size_t b=0; b<4; ++b) {
            for(size_t c=0; c<4; ++c) {
                const auto pos   = Tree::get_position_2d(a, b, c);
                const auto ox    = offset_x + (rsize * Tree::get_x_2d(a, b, c));
                const auto oy    = offset_y + (rsize * Tree::get_y_2d(a, b, c));
                const sf::Uint8 col = m->get_value(pos) * (255 / (4 * 4 * 4));

                if(rsize < window_width) {
                    sf::RectangleShape square { sf::Vector2f { static_cast<float>(rsize), static_cast<float>(rsize) } };
                    square.setFillColor(sf::Color { col, col, col, 255 } );
                    square.setPosition(ox, oy);
                    window.draw(square);
                }

                recursive_render(window, m->child(pos), rsize / 8.0f, ox, oy);
            }
        }
    }
}

template <typename Tree>
int zoom_render(sf::RenderWindow & window, const Tree & m, double speed)
{
    double rsize = 64;

    while(1) {
        recursive_render(window, &m, rsize, 0, 0);
        rsize *= speed;

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
int static_render(sf::RenderWindow & window, const Tree & m)
{
    double rsize = 64;

    while(1) {
        recursive_render(window, &m, rsize, 0, 0);

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


