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

constexpr unsigned init_capacity { 1 };

#pragma pack(1)
template <typename T>
class lmemvector
{
private:
    void init()
    {
        T * nb = new T[init_capacity];
        buf = nb;
    }

public:
    typedef T* iterator;

    T * buf;

    lmemvector() : buf { nullptr } 
    {}

    ~lmemvector()
    {
        if(buf != nullptr) {
            delete[] buf;
        }
    }

    T operator[](const unsigned n) const
    { return buf[n]; }
    T & operator[](const unsigned n)
    { return buf[n]; }

    void clear(const unsigned size)
    {
        if(size == 0) {
            buf = nullptr;
            return;
        }

        delete[] buf;
        buf = nullptr;
    }

    void erase(const unsigned position, const unsigned size)
    {
        assert(position < 64);
        assert(size < 64);

        std::memmove(buf + position, buf + position + 1, sizeof(T) * (size + 1 - position));
    }

    void reserve(const unsigned new_capacity)
    {
        assert(new_capacity <= 64);

        T * nb = new T[new_capacity];
        std::copy(&buf[0], &buf[new_capacity - 1], nb);
        delete[] buf;
        buf = nb;
    }

    void insert(const unsigned position, const T value, const unsigned size)
    {
        assert(position < 64);
        assert(size < 64);

        if(buf != nullptr) {
            reserve(size + 1);
        } else {
            init();
        }

        const iterator it = &buf[position];
        std::memmove(it + 1, it, sizeof(T) * (size - position));
        *it = value;
    }
};

};

#endif
