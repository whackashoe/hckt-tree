#ifndef LMEMVECTOR_H
#define LMEMVECTOR_H

#include <cstdint>
#include <algorithm> 
#include <cstring>

#pragma pack(1)
template <typename T>
class lmemvector
{
private:
    void init()
    {
        const size_t new_capacity = 1;
        T * nb = new T[new_capacity];
        _capacity = new_capacity;
        buf = nb;
    }

public:
    typedef T* iterator;
    typedef uint8_t size_type;

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

    T operator[](const size_t n) const
    { return buf[n]; }
    T & operator[](const size_t n)
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

    void erase(const size_type position)
    {
        --_size;
        std::memmove(buf + position, buf + position + 1, sizeof(T) * (_size + 1 - position));
    }

    void reserve(const size_t new_capacity)
    {
        T * nb = new T[new_capacity];
        std::copy(begin(), end(), nb);
        delete[] buf;
        _capacity = new_capacity;
        buf = nb;
    }

    void insert(const size_type position, const T value)
    {
        ++_size;
        if(_size >= _capacity) {
            if(_capacity > 0) {
                reserve(_capacity << 1);
            } else {
                init();
            }
        }

        const iterator it = begin() + position;
        std::memmove(it + 1, it, sizeof(T) * (_size - position - 1));
        *it = value;
    }
};

#endif
