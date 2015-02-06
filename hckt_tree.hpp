#ifndef HCKT_TREE_H
#define HCKT_TREE_H

#include <array>
#include <vector>
#include <bitset>
#include "direction.hpp"


template <typename ValueType>
class hckt_tree
{
protected:
    std::array<ValueType, 64> values;
    std::bitset<64> bitset;
    std::vector<hckt_tree*> children;

    size_t get_children_position(const size_t position) const
    {
        size_t c = 0;

        for(size_t i=0; i<position; ++i) {
            if(bitset[i]) {
                ++c;
            }
        }
        
        return c;
    }


public:
    hckt_tree() : values(), bitset{0}, children{}
    {}

    ~hckt_tree()
    {
        collapse();
    }

    static size_t get_position(size_t l1, size_t  l2, size_t l3)
    {
        return ((1 << 5) * ((l1 & 2) >> 1))
             + ((1 << 4) * (l1 & 1))
             + ((1 << 3) * ((l2 & 2) >> 1))
             + ((1 << 2) * (l2 & 1))
             + ((1 << 1) * ((l3 & 2) >> 1))
             + ((1 << 0) * (l3 & 1));
    }

    bool empty() const
    {
        return bitset.none();
    }

    void collapse()
    {
        for(auto child : children) {
            child->collapse();
        }

        children.clear();
        bitset.reset();
    }

    void insert(const size_t position, const ValueType value)
    {
        children.emplace(children.begin() + get_children_position(position), new hckt_tree<ValueType>());
        bitset.set(position);
        values[position] = value;
    }

    hckt_tree<ValueType> * leaf(const size_t position) const
    {
        return children[get_children_position(position)];
    }

    void set_value(const size_t position, const ValueType value)
    {
        values[position] = value;
    }

    ValueType get_value(const size_t position) const
    {
        return values[position];
    }
};


#endif
