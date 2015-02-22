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
#include <iostream>
#include <bitset>
#include "lmemvector.hpp"

namespace hckt
{

template <typename T>
class tree
{
typedef T value_type;

protected:
    std::bitset<64>                     chiset;   //is child set to this position
    std::bitset<64>                     inv_leaf; //opposite of leaf
    hckt::lmemvector<value_type>        values;
    hckt::lmemvector<tree<value_type>*> children;

public:

    tree() : chiset { 0 }, inv_leaf { 0xFFFFFFFFFFFFFFFF }, values { }, children { }
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

    std::uint64_t chidist() const
    {
        return (chiset.to_ullong() & inv_leaf.to_ullong());
    }

    unsigned children_amnt() const
    {
        return popcount(chidist());
    }

    unsigned leaf_amnt() const
    {
        return popcount(~inv_leaf.to_ullong());
    }

    unsigned get_children_position(const unsigned position) const
    {
        assert(position < 64);

        return position == 0 ? position : popcount(chidist() << (64 - position));
    }
    


    /*
     * check if we have any children
     */
    bool has_children() const
    {
        return chiset.any();
    }

    bool is_set(const unsigned position) const
    {
        assert(position < 64);
        return chiset[position];
    }

    bool is_leaf(const unsigned position) const
    {
        assert(position < 64);
        return !inv_leaf[position];
    }

    /*
     * destroy children
     */
    void collapse()
    {
        const unsigned c_amnt { children_amnt() };

        for(unsigned i=0; i<c_amnt; ++i) {
            children[i]->collapse();
        }

        children.clear(c_amnt);
        values.clear(c_amnt);
        chiset.reset();
    }

    /*
     * insert a tree into position of tree
     * position should be result of get_position
     */
    void insert(const unsigned position, const value_type value)
    {
        assert(position < 64);
        assert(! is_set(position));

        const unsigned cpos   { get_children_position(position) };
        const unsigned c_amnt { children_amnt() };

        children.insert(cpos, new tree<value_type>(), c_amnt);
        values.insert(cpos, value, c_amnt);
        chiset.set(position);
        inv_leaf.set(position);
    }

    /*
     * insert a leaf into position of tree
     * position should be result of get_position
     */
    void insert_leaf(const unsigned position, const value_type value)
    {
        assert(position < 64);
        assert(! is_set(position));

        const unsigned cpos   { get_children_position(position) };
        const unsigned c_amnt { children_amnt() };

        values.insert(cpos, value, c_amnt);
        chiset.set(position);
        inv_leaf.reset(position);
    }

    /*
     * removes an item from tree
     * position should be result of get_position
     */
    void remove(const unsigned position)
    {
        assert(position < 64);
        assert(is_set(position));
        assert(! is_leaf(position));

        const unsigned cpos   { get_children_position(position) };
        const unsigned c_amnt { children_amnt() };

        children[cpos]->collapse();
        children.erase(cpos);
        values.erase(cpos);
        chiset.reset(position);
    }

    /*
     * get child node
     * position should be result of get_position
     */
    tree<value_type> * child(const unsigned position) const
    {
        assert(position < 64);
        assert(is_set(position));
        assert(! is_leaf(position));

        const unsigned cpos { get_children_position(position) };

        return children[cpos];
    }

    /*
     * sets node to specific value
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    void set_value(const unsigned position, const value_type value)
    {
        assert(position < 64);
        assert(is_set(position));

        const unsigned cpos { get_children_position(position) };

        values[cpos] = value;
    }

    /*
     * gets value from specific position
     * note: this does not check whether a value has been inserted here yet
     * position should be result of get_position
     */
    value_type get_value(const unsigned position) const
    {
        assert(position < 64);

        const unsigned cpos { get_children_position(position) };

        return values[cpos];
    }


    /************************************************
     *
     * BENCHMARKING CODE
     *
     ***********************************************/

    std::size_t calculate_memory_size() const
    {
        const unsigned c_amnt { children_amnt() };
        const unsigned l_amnt { leaf_amnt() };
        const unsigned v_amnt { c_amnt + l_amnt };

        std::size_t size {
              sizeof(chiset)
            + sizeof(inv_leaf)
            + sizeof(values)   + (l_amnt * sizeof(value_type))
            + sizeof(children) + (c_amnt * sizeof(tree<value_type>*))
        };

        for(unsigned i=0; i<c_amnt; ++i) {
            size += children[i]->calculate_memory_size();
        }

        return size;
    }

    std::size_t calculate_children_amnt() const
    {
        std::size_t amount { children_amnt() };

        for(unsigned i=0, c_amnt = children_amnt(); i<c_amnt; ++i) {
            amount += children[i]->calculate_children_amnt();
        }

        return amount;
    }

    std::size_t calculate_leaf_amount() const
    {
        std::size_t amount { leaf_amnt() };

        for(unsigned i=0, c_amnt = children_amnt(); i<c_amnt; ++i) {
            amount += children[i]->calculate_leaf_amount();
        }

        return amount;
    }

    void mem_usage_info() const
    {
        const unsigned memsize { calculate_memory_size() };
        const unsigned c_amnt  { calculate_children_amnt() };
        const unsigned l_amnt  { calculate_leaf_amount() };
        const unsigned v_amnt  { c_amnt + l_amnt };

        std::cout << "total:     " << memsize << std::endl;
        
        std::cout << "tree-size: " << sizeof(tree<value_type>) << std::endl;
        std::cout << "val-size:  " << sizeof(value_type) << std::endl;
        
        std::cout << "values:    " << v_amnt << std::endl;
        std::cout << "children:  " << c_amnt << std::endl;
        std::cout << "leaves:    " << l_amnt << std::endl;

        std::cout << "per-val:   " << (static_cast<float>(memsize) / v_amnt) << std::endl;
        std::cout << "overhead:  " << (static_cast<float>(memsize - (v_amnt * sizeof(value_type))) / v_amnt) << std::endl;
    }

};

};

#endif
