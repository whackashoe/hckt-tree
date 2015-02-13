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
    hckt::lmemvector<ValueType>        values;
    hckt::lmemvector<tree<ValueType>*> children;

public:
    tree() : bitset { 0 }, values { }, children { }
    {
    }

    ~tree()
    {
        collapse();
    }

    //counts number of set bits
    static inline int popcount(std::uint64_t x)
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
    
    std::size_t get_children_position(const std::size_t position) const
    {
        assert(position >= 0 && position < 64);

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


    /*
     * 2d get position
     * convert three part positions to memory location
     * as input: (left = 0 | right = 1) + (top = 0 | bottom = 2)
     *
     * result will be:
     * y1:x1:y2:x2:y3:x3
     *
     */
    static std::size_t get_position_2d(const std::size_t d1, const std::size_t d2, const std::size_t d3)
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

    static std::size_t get_x_2d(const std::size_t d1, const std::size_t d2, const std::size_t d3)
    {
        assert(d1 >= 0 && d1 < 4);
        assert(d2 >= 0 && d2 < 4);
        assert(d3 >= 0 && d3 < 4);

        return ((d1 & 1) << 0)
             + ((d2 & 1) << 1)
             + ((d3 & 1) << 2);
    }

    static std::size_t get_y_2d(const std::size_t d1, const std::size_t d2, const std::size_t d3)
    {
        assert(d1 >= 0 && d1 < 4);
        assert(d2 >= 0 && d2 < 4);
        assert(d3 >= 0 && d3 < 4);

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
    static std::size_t get_position_3d(const std::size_t d1, const std::size_t d2)
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

    static std::size_t get_x_3d(const std::size_t d1, const std::size_t d2)
    {
        assert(d1 >= 0 && d1 < 8);
        assert(d2 >= 0 && d2 < 8);

        return ((d1 & 1) << 0)
             + ((d2 & 1) << 1);
    }

    static std::size_t get_y_3d(const std::size_t d1, const std::size_t d2)
    {
        assert(d1 >= 0 && d1 < 8);
        assert(d2 >= 0 && d2 < 8);

        return ((d1 & 2) >> 1 << 0)
             + ((d2 & 2) >> 1 << 1);
    }

    static std::size_t get_z_3d(const std::size_t d1, const std::size_t d2)
    {
        assert(d1 >= 0 && d1 < 8);
        assert(d2 >= 0 && d2 < 8);

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

    bool isset(const std::size_t position) const
    {
        assert(position >= 0 && position < 64);

        return bitset[position];
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
    void insert(const std::size_t position, const ValueType value)
    {
        assert(position >= 0 && position < 64);

        const std::size_t cpos { get_children_position(position) };

        children.insert(cpos, new tree<ValueType>());
        values.insert(cpos, value);
        bitset.set(position);
    }

    void insert_direct(const std::size_t position, const std::size_t cpos, const ValueType value)
    {
        assert(position >= 0 && position < 64);
        assert(cpos     >= 0 && cpos     < 64);

        children.insert(cpos, new tree<ValueType>());
        values.insert(cpos, value);
        bitset.set(position);
    }

    /*
     * removes an item from tree
     * position should be result of get_position
     */
    void remove(const std::size_t position)
    {
        assert(position >= 0 && position < 64);

        const std::size_t cpos { get_children_position(position) };

        children[cpos]->collapse();
        children.erase(cpos);
        values.erase(cpos);
        bitset.reset(position);
    }

    void remove_direct(const std::size_t position, const std::size_t cpos)
    {
        assert(position >= 0 && position < 64);
        assert(cpos     >= 0 && cpos     < 64);

        children[cpos]->collapse();
        children.erase(cpos);
        values.erase(cpos);
        bitset.reset(position);
    }

    /*
     * get child node 
     * position should be result of get_position
     */
    tree<ValueType> * child(const std::size_t position) const
    {
        assert(position >= 0 && position < 64);

        return child_direct(get_children_position(position));
    }

    tree<ValueType> * child_direct(const std::size_t cpos) const
    {
        assert(cpos >= 0 && cpos < 64);

        return children[cpos];
    }


    /*
     * sets node to specific value
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    void set_value(const std::size_t position, const ValueType value)
    {
        assert(position >= 0 && position < 64);

        set_value_direct(get_children_position(position));
    }

    void set_value_direct(const std::size_t cpos, const ValueType value)
    {
        assert(cpos >= 0 && cpos < 64);

        values[cpos] = value;
    }

    /*
     * gets value from specific position
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    ValueType get_value(const std::size_t position) const
    {
        assert(position >= 0 && position < 64);

        return get_value_direct(get_children_position(position));
    }

    ValueType get_value_direct(const std::size_t cpos) const
    {
        assert(cpos >= 0 && cpos < 64);

        return values[cpos];
    }
};

};

#endif
