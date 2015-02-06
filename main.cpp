#include <iostream>
#include <bitset>
#include "hckt_tree.hpp"
#include "direction.hpp"

int main(int argc, char ** argv)
{
    hckt_tree<int> m;
    auto pos_a = decltype(m)::get_position(TOPRIGHT, TOPRIGHT, BOTRIGHT);
    auto pos_b = decltype(m)::get_position(BOTRIGHT, BOTLEFT, TOPRIGHT);
    m.insert(pos_a, 10);
    std::cout << m.get_value(pos_a) << std::endl;

    m.leaf(pos_a)->insert(pos_b, 15);
    std::cout << m.leaf(pos_a)->get_value(pos_b) << std::endl;

    std::bitset<6> test(pos_b);
    std::cout << test.to_string() << std::endl;
}
