#include <hckt/tree.hpp>
#include <hckt/bench.hpp>
#include <hckt/lmemvector.hpp>

#include "inc_populate_2d_a.cpp"

int main()
{
    hckt::tree<uint32_t> m;
    populate(m, 3);
    hckt::bench::mem_usage_info(&m);

    return 0;
}

