#include <chrono>
#include <iostream>

#include <hckt/tree.hpp>
#include <hckt/lmemvector.hpp>
#include "inc_populate_2d_a.cpp"

int main()
{
    for(size_t i=0; i<8; ++i) {
        std::cout << "DEPTH " << i << std::endl;
        auto pstart = std::chrono::steady_clock::now();
        hckt::tree<short> m;
        populate(m, i);
        auto pend = std::chrono::steady_clock::now();
        auto pdiff = pend-pstart;
        auto mstart = std::chrono::steady_clock::now();
        m.mem_usage_info();
        auto mend = std::chrono::steady_clock::now();
        auto mdiff = mend - mstart;
        std::cout << std::endl;
        std::cout << "poptime: " << std::chrono::duration<double, std::milli>(pdiff).count() << " ms" << std::endl;
        std::cout << "memtime: " << std::chrono::duration<double, std::milli>(mdiff).count() << " ms" << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    return 0;
}

