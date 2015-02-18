#include <chrono>
#include <iostream>

#include <hckt/tree.hpp>
#include <hckt/lmemvector.hpp>
#include "inc_populate_2d_a.cpp"

int main()
{
    for(size_t i=1; i<7; ++i) {
        std::cout << "DEPTH " << i << std::endl;
        auto start = std::chrono::steady_clock::now();
        hckt::tree<uint32_t> m;
        populate(m, i);
        m.mem_usage_info();
        auto end = std::chrono::steady_clock::now();
        auto diff = end-start;
        std::cout << std::endl;
        std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    return 0;
}

