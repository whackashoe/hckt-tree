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

#ifndef LMEMVECTOR_H
#define LMEMVECTOR_H

#include <cstdint>
#include <algorithm> 
#include <cstring>

namespace hckt
{

constexpr std::size_t init_capacity { 2 };
constexpr double      exp_inc { 1.6 };

#pragma pack(1)
template <typename T>
class lmemvector
{
private:
    void init()
    {
        T * nb = new T[init_capacity];
        _capacity = init_capacity;
        buf = nb;
    }

public:
    typedef T* iterator;
    typedef std::uint8_t size_type;

    T * buf;
    size_type _capacity;
    size_type _size;

    lmemvector() : buf { nullptr }, _capacity { 0 }, _size { 0 } 
    {}

    ~lmemvector()
    {
        if(_size != 0) {
            delete[] buf;
        }
    }

    lmemvector(const lmemvector & rval)
    {
        capacity = rval.capacity;
        size = rval.size;
        buf = new T[capacity];
        std::copy(rval.begin(), rval.end(), buf.begin());
    }

    lmemvector operator=(const lmemvector & rval)
    {
        if(buf != nullptr) {
            delete[] buf;
        }

        capacity = rval.capacity;
        size = rval.size;
        buf = new T[capacity];
        std::copy(rval.begin(), rval.end(), buf.begin());

        return *this;
    }

    T operator[](const std::size_t n) const
    { return buf[n]; }
    T & operator[](const std::size_t n)
    { return buf[n]; }

    iterator begin()
    { return buf; }
    iterator begin() const
    { return buf; }

    iterator end()
    { return buf + _size; }
    iterator end() const
    { return buf + _size; }

    T & front()
    { return buf[0]; }
    T & front() const
    { return buf[0]; }

    T & back()
    { return buf[_size - 1]; }
    T & back() const
    { return buf[_size - 1]; }

    size_type size() const
    { return _size; }

    size_type capacity() const
    { return _capacity; }

    void clear()
    {
        delete[] buf;
        _capacity = 0;
        _size = 0;
    }

    void erase(const std::size_t position)
    {
        assert(position < std::numeric_limits<size_type>::max());

        --_size;
        std::memmove(buf + position, buf + position + 1, sizeof(T) * (_size + 1 - position));
    }

    void reserve(const std::size_t new_capacity)
    {
        assert(new_capacity < std::numeric_limits<size_type>::max());

        T * nb = new T[new_capacity];
        std::copy(begin(), end(), nb);
        delete[] buf;
        _capacity = new_capacity;
        buf = nb;
    }

    void insert(const std::size_t position, const T value)
    {
        assert(position < std::numeric_limits<size_type>::max());

        ++_size;
        if(_size >= _capacity) {
            if(_capacity > 0) {
                reserve(_capacity * exp_inc);
            } else {
                init();
            }
        }

        const iterator it = begin() + position;
        std::memmove(it + 1, it, sizeof(T) * (_size - position - 1));
        *it = value;
    }
};

};

#endif
