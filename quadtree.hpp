#ifndef QUADTREE_H
#define QUADTREE_H

#include <array>
#include <vector>
#include <bitset>
#include "direction.hpp"


template <unsigned int DepthStep, typename ValueType>
class Quadtree
{
protected:
    std::array<ValueType, (1 << DepthStep * 2)> values;
    std::bitset<(1 << DepthStep * 2)> bitset;
    std::vector<Quadtree*> children;

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
    Quadtree() : values(), bitset{0}, children{}
    {}

    ~Quadtree()
    {
        collapse();
    }

    static size_t get_position(size_t l1, size_t  l2, size_t l3)
    {
        return l1
            + (1 << (l2 & 1 * 2))
            + (1 << (((l2 & 2) >> 1) * 3))
            + (1 << (l3 & 1 * 4))
            + (1 << (((l3 & 2) >> 1) * 5));
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
        children.emplace(children.begin() + get_children_position(position), new Quadtree<DepthStep, ValueType>());
        bitset.set(position);
        values[position] = value;
    }

    Quadtree<DepthStep, ValueType> * leaf(const size_t position) const
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
