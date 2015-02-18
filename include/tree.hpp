/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Jett
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef HCKT_TREE_H
#define HCKT_TREE_H

#include <cassert>
#include <vector>
#include <bitset>
#include "lmemvector.hpp"

namespace hckt
{

template <typename ValueType>
class tree
{
protected:
    std::bitset<64>                    bitset;
    std::bitset<64>                    leaf;
    hckt::lmemvector<ValueType>        values;
    hckt::lmemvector<tree<ValueType>*> children;

public:
    tree() : bitset { 0 }, leaf { 0 }, values { }, children { }
    {
    }

    ~tree()
    {
        collapse();
    }

    //counts number of set bits
    static inline unsigned popcount(std::uint64_t x)
    {
#ifdef __SSE4_2__
        return _popcnt64(x);
#else
        constexpr std::uint64_t m1  { 0x5555555555555555 };
        constexpr std::uint64_t m2  { 0x3333333333333333 };
        constexpr std::uint64_t m4  { 0x0f0f0f0f0f0f0f0f };
        constexpr std::uint64_t h01 { 0x0101010101010101 };

        x -= (x >> 1) & m1;
        x  = (x & m2) + ((x >> 2) & m2);
        x  = (x + (x >> 4)) & m4;
     
        return (x * h01) >> 56;
#endif
    }
    
    unsigned get_children_position(const unsigned position) const
    {
        assert(position < 64);

        //we know it will be 0
        //plus - we cant shift by 64 without ub
        if(position == 0) {
            return 0;
        }

        return popcount(bitset.to_ullong() << (64 - position));
    }
    
    std::size_t calculate_memory_size() const
    {
        std::size_t size { 
              sizeof(bitset)
            + sizeof(leaf)
            + sizeof(values)   + (values.capacity()   * sizeof(ValueType))
            + sizeof(children) + (children.capacity() * sizeof(tree<ValueType>*))
        };

        for(auto child : children) {
            size += child->calculate_memory_size();
        }
        
        return size;
    }

    std::size_t calculate_children_amount() const
    {
        std::size_t amount { popcount(bitset.to_ullong()) };

        for(auto child : children) {
            amount += child->calculate_children_amount();
        }
        
        return amount;
    }

    void mem_usage_info()
    {
        auto memsize = calculate_memory_size();
        auto camnt = calculate_children_amount();
        std::cout << "total:     " << memsize << std::endl;
        std::cout << "tree-size: " << sizeof(tree<ValueType>) << std::endl;
        std::cout << "children:  " << camnt << std::endl;
        std::cout << "per-child: " << (static_cast<float>(memsize) / camnt) << std::endl;
        std::cout << "val-size:  " << sizeof(ValueType) << std::endl;
        std::cout << "overhead:  " << (static_cast<float>(memsize - (camnt * sizeof(ValueType))) / camnt) << std::endl;
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
    static unsigned get_position_2d(const unsigned d1, const unsigned d2, const unsigned d3)
    {
        assert(d1 < 4);
        assert(d2 < 4);
        assert(d3 < 4);

        return ((1 << 0) * ((d1 & 2) >> 1))
             + ((1 << 1) *  (d1 & 1))
             + ((1 << 2) * ((d2 & 2) >> 1))
             + ((1 << 3) *  (d2 & 1))
             + ((1 << 4) * ((d3 & 2) >> 1))
             + ((1 << 5) *  (d3 & 1));
    }

    static unsigned get_x_2d(const unsigned d1, const unsigned d2, const unsigned d3)
    {
        assert(d1 < 4);
        assert(d2 < 4);
        assert(d3 < 4);

        return ((d1 & 1) << 0)
             + ((d2 & 1) << 1)
             + ((d3 & 1) << 2);
    }

    static unsigned get_y_2d(const unsigned d1, const unsigned d2, const unsigned d3)
    {
        assert(d1 < 4);
        assert(d2 < 4);
        assert(d3 < 4);

        return ((d1 & 2) >> 1 << 0)
             + ((d2 & 2) >> 1 << 1)
             + ((d3 & 2) >> 1 << 2);
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
    static unsigned get_position_3d(const unsigned d1, const unsigned d2)
    {
        assert(d1 < 8);
        assert(d2 < 8);

        return ((1 << 0) * ((d1 & 4) >> 2))
             + ((1 << 1) * ((d1 & 2) >> 1))
             + ((1 << 2) *  (d1 & 1))
             + ((1 << 3) * ((d2 & 4) >> 2))
             + ((1 << 4) * ((d2 & 2) >> 1))
             + ((1 << 5) *  (d2 & 1));
    }

    static unsigned get_x_3d(const unsigned d1, const unsigned d2)
    {
        assert(d1 < 8);
        assert(d2 < 8);

        return ((d1 & 1) << 0)
             + ((d2 & 1) << 1);
    }

    static unsigned get_y_3d(const unsigned d1, const unsigned d2)
    {
        assert(d1 < 8);
        assert(d2 < 8);

        return ((d1 & 2) >> 1 << 0)
             + ((d2 & 2) >> 1 << 1);
    }

    static unsigned get_z_3d(const unsigned d1, const unsigned d2)
    {
        assert(d1 < 8);
        assert(d2 < 8);

        return ((d1 & 4) >> 2 << 0)
             + ((d2 & 4) >> 2 << 1);
    }


    /*
     * check if we have any children
     */
    bool empty() const
    {
        return bitset.none();
    }

    bool isset(const unsigned position) const
    {
        assert(position < 64);

        return bitset[position];
    }

    bool is_leaf(const unsigned position) const
    {
        assert(position < 64);
        return leaf[position];
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
     * insert an item unsignedo position of tree
     * position should be result of get_position
     */
    void insert(const unsigned position, const ValueType value)
    {
        assert(position < 64);
        assert(! isset(position));

        const unsigned cpos { get_children_position(position) };

        children.insert(cpos, new tree<ValueType>());
        values.insert(cpos, value);
        bitset.set(position);
    }

    void insert_direct(const unsigned position, const unsigned cpos, const ValueType value)
    {
        assert(position < 64);
        assert(! isset(position));
        assert(cpos     < 64);

        children.insert(cpos, new tree<ValueType>());
        values.insert(cpos, value);
        bitset.set(position);
    }

    /*
     * removes an item from tree
     * position should be result of get_position
     */
    void remove(const unsigned position)
    {
        assert(position < 64);
        assert(isset(position));

        const unsigned cpos { get_children_position(position) };

        children[cpos]->collapse();
        children.erase(cpos);
        values.erase(cpos);
        bitset.reset(position);
    }

    void remove_direct(const unsigned position, const unsigned cpos)
    {
        assert(position < 64);
        assert(isset(position));
        assert(cpos     < 64);

        children[cpos]->collapse();
        children.erase(cpos);
        values.erase(cpos);
        bitset.reset(position);
    }

    /*
     * get child node 
     * position should be result of get_position
     */
    tree<ValueType> * child(const unsigned position) const
    {
        assert(position < 64);
        assert(isset(position));
   
        return child_direct(get_children_position(position));
    }

    tree<ValueType> * child_direct(const unsigned cpos) const
    {
        assert(cpos < 64);

        return children[cpos];
    }


    /*
     * sets node to specific value
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    void set_value(const unsigned position, const ValueType value)
    {
        assert(position < 64);
        assert(isset(position));

        set_value_direct(get_children_position(position));
    }

    void set_value_direct(const unsigned cpos, const ValueType value)
    {
        assert(cpos < 64);

        values[cpos] = value;
    }

    /*
     * gets value from specific position
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    ValueType get_value(const unsigned position) const
    {
        assert(position < 64);

        return get_value_direct(get_children_position(position));
    }

    ValueType get_value_direct(const unsigned cpos) const
    {
        assert(cpos < 64);
        assert(isset(cpos));

        return values[cpos];
    }
};

};

#endif
