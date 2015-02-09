#ifndef HCKT_TREE_H
#define HCKT_TREE_H

#include <cassert>
#include <vector>
#include <bitset>
#include "lmemvector.hpp"

template <typename ValueType>
class hckt_tree
{
protected:
    std::bitset<64> bitset;
    lmemvector<ValueType> values;
    lmemvector<hckt_tree<ValueType>*> children;

    size_t get_children_position(const size_t position) const
    {
        assert(position >= 0 && position < 64);

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
        size_t size { sizeof(bitset)
                    + sizeof(values)   + (values.capacity()   * sizeof(ValueType))
                    + sizeof(children) + (children.capacity() * sizeof(hckt_tree<ValueType>*))
        };

        for(auto child : children) {
            size += child->calculate_memory_size();
        }
        
        return size;
    }


    /*
     * 2d get position
     * convert three part positions to memory location
     * as input: (left = 0 | right = 1) + (top = 0 | bottom = 2)
     *
     * result will be:
     * y1:x1:y2:x2:y3:x3
     *
     */
    static size_t get_position_2d(const size_t d1, const size_t d2, const size_t d3)
    {
        assert(d1 >= 0 && d1 < 4);
        assert(d2 >= 0 && d2 < 4);
        assert(d3 >= 0 && d3 < 4);

        return ((1 << 0) * ((d1 & 2) >> 1))
             + ((1 << 1) *  (d1 & 1))
             + ((1 << 2) * ((d2 & 2) >> 1))
             + ((1 << 3) *  (d2 & 1))
             + ((1 << 4) * ((d3 & 2) >> 1))
             + ((1 << 5) *  (d3 & 1));
    }

    static size_t get_x_2d(const size_t d1, const size_t d2, const size_t d3)
    {
        assert(d1 >= 0 && d1 < 4);
        assert(d2 >= 0 && d2 < 4);
        assert(d3 >= 0 && d3 < 4);

        return ((d1 & 1) << 0) + ((d2 & 1) << 1) + ((d3 & 1) << 2);
    }

    static size_t get_y_2d(const size_t d1, const size_t d2, const size_t d3)
    {
        assert(d1 >= 0 && d1 < 4);
        assert(d2 >= 0 && d2 < 4);
        assert(d3 >= 0 && d3 < 4);

        return ((d1 & 2) >> 1 << 0) + ((d2 & 2) >> 1 << 1) + ((d3 & 2) >> 1 << 2);
    }

    /*
     * 3d get position
     * convert two part positions to memory location
     * as input: (left = 0 | right = 1) + (top = 0 | bottom = 2) + (in = 0 | out = 4)
     *
     * result will be:
     * z1:y1:x1:z2:y2:x2
     *
     */
    static size_t get_position_3d(const size_t d1, const size_t d2)
    {
        assert(d1 >= 0 && d1 < 8);
        assert(d2 >= 0 && d2 < 8);

        return ((1 << 0) * ((d1 & 4) >> 2))
             + ((1 << 1) * ((d1 & 2) >> 1))
             + ((1 << 2) *  (d1 & 1))
             + ((1 << 3) * ((d2 & 4) >> 2))
             + ((1 << 4) * ((d2 & 2) >> 1))
             + ((1 << 5) *  (d2 & 1));
    }

    static size_t get_x_3d(const size_t d1, const size_t d2)
    {
        assert(d1 >= 0 && d1 < 8);
        assert(d2 >= 0 && d2 < 8);

        return ((d1 & 1) << 0) + ((d2 & 1) << 1);
    }

    static size_t get_y_3d(const size_t d1, const size_t d2)
    {
        assert(d1 >= 0 && d1 < 8);
        assert(d2 >= 0 && d2 < 8);

        return ((d1 & 2) >> 1 << 0) + ((d2 & 2) >> 1 << 1);
    }

    static size_t get_z_3d(const size_t d1, const size_t d2)
    {
        assert(d1 >= 0 && d1 < 8);
        assert(d2 >= 0 && d2 < 8);

        return ((d1 & 4) >> 2 << 0) + ((d2 & 4) >> 2 << 1);
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
        const size_t cpos { get_children_position(position) };
        assert(cpos >= 0 && cpos < 64);

        children.insert(cpos, new hckt_tree<ValueType>());
        values.insert(cpos, value);
        bitset.set(position);
    }

    /*
     * removes an item from tree
     * position should be result of get_position
     */
    void remove(const size_t position)
    {
        const size_t cpos { get_children_position(position) };
        assert(cpos >= 0 && cpos < 64);
        
        children[cpos]->collapse();
        children.erase(cpos);
        values.erase(cpos);
        bitset.reset(position);
    }

    /*
     * get child node 
     * position should be result of get_position
     */
    hckt_tree<ValueType> * leaf(const size_t position) const
    {
        const size_t cpos { get_children_position(position) };
        assert(cpos >= 0 && cpos < 64);
        
        return children[cpos];
    }

    /*
     * sets node to specific value
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    void set_value(const size_t position, const ValueType value)
    {
        const size_t cpos { get_children_position(position) };
        assert(cpos >= 0 && cpos < 64);
        
        values[cpos] = value;
    }

    /*
     * gets value from specific position
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    ValueType get_value(const size_t position) const
    {
        const size_t cpos { get_children_position(position) };
        assert(cpos >= 0 && cpos < 64);
        
        return values[cpos];
    }
};


#endif
