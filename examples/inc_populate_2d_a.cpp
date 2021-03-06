#include <hckt/util.hpp>

template <typename Tree>
void recursive_populate(Tree * m, const int depth, const int max_depth)
{
    for(size_t a=0; a<4; ++a) {
        for(size_t b=0; b<4; ++b) {
            for(size_t c=0; c<4; ++c) {
                const auto pos = hckt::get_position_2d(a, b, c);
                const auto alg = (1 + a) * (1 + b) * (1 + c);


                if(depth < max_depth) {
                    m->insert(pos, alg);
                    if(alg % 7 == 1) {
                        recursive_populate(m->child(pos), depth + 1, max_depth);
                    }
                } else {
                    m->insert_leaf(pos, alg);
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
