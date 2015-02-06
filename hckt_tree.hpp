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
        size_t c { 0 };

        for(size_t i=0; i<position; ++i) {
            if(bitset[i]) {
                ++c;
            }
        }
        
        return c;
    }


public:
    hckt_tree() : values { { } }, bitset { 0 }, children { }
    {
    }

    ~hckt_tree()
    {
        collapse();
    }

    /*
     * convert three part positions to memory location
     * as input: (left = 0 | right = 1) + (top = 0 | bottom = 2)
     *
     * result will be:
     * y1:x1:y2:x2:y3:x3
     *
     */
    static size_t get_position(size_t d1, size_t d2, size_t d3)
    {
        return ((1 << 5) * ((d1 & 2) >> 1))
             + ((1 << 4) * (d1 & 1))
             + ((1 << 3) * ((d2 & 2) >> 1))
             + ((1 << 2) * (d2 & 1))
             + ((1 << 1) * ((d3 & 2) >> 1))
             + ((1 << 0) * (d3 & 1));
    }

    /*
     * check if we have any children
     */
    bool empty() const
    {
        return bitset.none();
    }

    /*
     * destroy children
     * note: this does not delete prior values in array
     */
    void collapse()
    {
        for(auto child : children) {
            child->collapse();
        }

        children.clear();
        bitset.reset();
    }

    /*
     * insert an item into position of tree
     * position should be result of get_position
     */
    void insert(const size_t position, const ValueType value)
    {
        children.emplace(children.begin() + get_children_position(position), new hckt_tree<ValueType>());
        bitset.set(position);
        values[position] = value;
    }

    /*
     * get child node 
     * position should be result of get_position
     */
    hckt_tree<ValueType> * leaf(const size_t position) const
    {
        return children[get_children_position(position)];
    }

    /*
     * sets node to specific value
     * position should be result of get_position
     */
    void set_value(const size_t position, const ValueType value)
    {
        values[position] = value;
    }

    /*
     * gets value from specific position
     * note: this does not check / concern itself if this has been collapsed
     * if so the data is stale
     * position should be result of get_position
     */
    ValueType get_value(const size_t position) const
    {
        return values[position];
    }
};


#endif
