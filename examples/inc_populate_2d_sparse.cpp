#include <hckt/util.hpp>

template <typename Tree>
void recursive_populate(Tree * m, const int depth, const int max_depth)
{
    if(depth >= max_depth) {
        return;
    }

    for(size_t a=0; a<4; ++a) {
        for(size_t b=0; b<4; ++b) {
            for(size_t c=0; c<4; ++c) {
                const auto pos = hckt::get_position_2d(a, b, c);
                m->insert(pos, 8 + rand() % 8);

                if(pos == 0) { 
                    recursive_populate(m->child(pos), depth + 1, max_depth);
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
