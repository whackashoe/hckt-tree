#include <iostream>
#include "quadtree.hpp"

int main(int argc, char ** argv)
{
    Quadtree<3, int> m;
    m.insert(0, 10);
    std::cout << m.get_value(0) << std::endl;
    m.leaf(0)->insert(1, 15);
    std::cout << m.leaf(0)->get_value(1) << std::endl;
}
