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
    std::bitset<64> bitset;
    std::vector<ValueType> values;
    std::vector<hckt_tree<ValueType>*> children;

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
    hckt_tree() : bitset { 0 }, values { }, children { }
    {
    }

    ~hckt_tree()
    {
        collapse();
    }
    
    size_t calculate_memory_size() const
    {
        size_t size = sizeof(bitset)
                    + sizeof(values) + (values.size() * sizeof(ValueType))
                    + sizeof(children) + (children.size() * sizeof(hckt_tree<ValueType>*));

        for(auto child : children) {
            size += child->calculate_memory_size();
        }
        
        return size;
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
        return ((1 << 0) * ((d1 & 2) >> 1))
             + ((1 << 1) *  (d1 & 1))
             + ((1 << 2) * ((d2 & 2) >> 1))
             + ((1 << 3) *  (d2 & 1))
             + ((1 << 4) * ((d3 & 2) >> 1))
             + ((1 << 5) *  (d3 & 1));
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
     */
    void collapse()
    {
        for(auto child : children) {
            child->collapse();
        }

        children.clear();
        values.clear();
        bitset.reset();
    }

    /*
     * insert an item into position of tree
     * position should be result of get_position
     */
    void insert(const size_t position, const ValueType value)
    {
        auto cpos = get_children_position(position);
        children.emplace(children.begin() + cpos, new hckt_tree<ValueType>());
        values.emplace(values.begin() + cpos, value);
        bitset.set(position);
    }

    /*
     * removes an item from tree
     * position should be result of get_position
     */
    void remove(const size_t position)
    {
        const size_t cpos = get_children_position(position);
        children[cpos]->collapse();
        children.erase(children.begin() + cpos);
        values.erase(values.begin() + cpos);
        bitset.reset(position);
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
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    void set_value(const size_t position, const ValueType value)
    {
        values[get_children_position(position)] = value;
    }

    /*
     * gets value from specific position
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    ValueType get_value(const size_t position) const
    {
        return values[get_children_position(position)];
    }
};


#endif
