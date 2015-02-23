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

#pragma pack
template <typename T>
class lmemvector
{
typedef T* iterator;
typedef T value_type;

private:
    void init()
    {
        iterator nb = new value_type[init_capacity];
        buf = nb;
    }

public:
    iterator buf;

    lmemvector() : buf { nullptr }
    {}

    ~lmemvector()
    {
        if(buf != nullptr) {
            delete[] buf;
        }
    }

    T operator[](const unsigned n) const
    {
        return buf[n];
    }

    T & operator[](const unsigned n)
    {
        return buf[n];
    }

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

        std::memmove(buf + position, buf + position + 1, sizeof(value_type) * (size + 1 - position));
    }

    void reserve(const unsigned new_capacity)
    {
        assert(new_capacity <= 64);

        iterator nb = new value_type[new_capacity];
        std::copy(&buf[0], &buf[new_capacity - 1], nb);
        delete[] buf;
        buf = nb;
    }

    void insert(const unsigned position, const value_type value, const unsigned size)
    {
        assert(position < 64);
        assert(size < 64);

        if(buf != nullptr) {
            reserve(size + 1);
        } else {
            init();
        }

        const iterator it = &buf[position];
        std::memmove(it + 1, it, sizeof(value_type) * (size - position));
        *it = value;
    }
};

};

#endif
