#ifndef HCKT_BENCH_H
#define HCKT_BENCH_H

#include <cstdint>
#include <iostream>

namespace hckt
{

class bench
{
public:
    template <typename Tree>
    static std::size_t calculate_memory_size(const Tree * t)
    {
        std::size_t size { 
              sizeof(t->bitset)
            + sizeof(t->leaf)
            + sizeof(t->values)   + (t->values.capacity()   * sizeof(typename Tree::value_type))
            + sizeof(t->children) + (t->children.capacity() * sizeof(t))
        };

        for(auto child : t->children) {
            size += calculate_memory_size(child);
        }
        
        return size;
    }

    template <typename Tree>
    static std::size_t calculate_children_amount(const Tree * t)
    {
        std::size_t amount { t->popcount(t->bitset.to_ullong()) };

        for(auto child : t->children) {
            amount += calculate_children_amount(child);
        }
        
        return amount;
    }

    template <typename Tree>
    static std::size_t calculate_leaf_amount(const Tree * t)
    {
        std::size_t amount { t->popcount(t->leaf.to_ullong()) };

        for(auto child : t->children) {
            amount += calculate_leaf_amount(child);
        }
        
        return amount;
    }

    template <typename Tree>
    static void mem_usage_info(const Tree * t)
    {
        auto memsize = calculate_memory_size(t);
        auto c_amnt  = calculate_children_amount(t);
        auto l_amnt  = calculate_leaf_amount(t);

        std::cout << "total:     " << memsize << std::endl;
        std::cout << "tree-size: " << sizeof(Tree) << std::endl;
        std::cout << "children:  " << c_amnt << std::endl;
        std::cout << "leaves:    " << l_amnt << std::endl;
        std::cout << "per-child: " << (static_cast<float>(memsize) / c_amnt) << std::endl;
        std::cout << "val-size:  " << sizeof(typename Tree::value_type) << std::endl;
        std::cout << "overhead:  " << (static_cast<float>(memsize - (c_amnt * sizeof(typename Tree::value_type))) / c_amnt) << std::endl;
    }
};

};

#endif

