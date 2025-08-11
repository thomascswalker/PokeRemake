#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "String.h"

/**
 * @brief Wrapper around std::vector to provide additional functionality.
 * @tparam T The element type.
 */
template <typename T>
class Array
{
    std::vector<T> mData;

public:
    Array() = default;

    Array(const Array& Other)
    {
        mData = Other.mData;
    }

    Array(const std::vector<T>& Other)
    {
        mData = Other;
    }

    Array(std::initializer_list<T> Other)
    {
        mData = std::vector<T>(Other.begin(), Other.end());
    }

    ~Array() = default;

    T* begin()
    {
        return &mData[0];
    }

    T* end()
    {
        return &mData[mData.size()];
    }

    size_t Size() const
    {
        return mData.size();
    }

    void Add(const T& Value)
    {
        mData.push_back(Value);
    }

    void AddFront(const T& Value)
    {
        mData.emplace(mData.begin(), Value);
    }

    void RemoveAt(size_t Index)
    {
        mData.erase(mData.begin() + Index);
    }

    void Remove(const T& Value)
    {
        mData.erase(std::remove(mData.begin(), mData.end(), Value), mData.end());
    }

    size_t Find(const T& Value)
    {
        return std::find(mData.begin(), mData.end(), Value);
    }

    bool Contains(const T& Value)
    {
        return std::find(mData.begin(), mData.end(), Value) != mData.end();
    }

    template <typename P>
    Array Filter(const P& Predicate)
    {
        std::vector<T> Result;
        std::copy_if(mData.begin(), mData.end(), std::back_inserter(Result), Predicate);
        return Result;
    }

    std::string ToString() const
    {
        auto Result = String::Join(mData, ",");
        return std::format("[{}]", Result.c_str());
    }

    Array& operator=(const Array& Other)
    {
        mData = Other.mData;
        return *this;
    }

    bool operator==(const Array& Other) const
    {
        return mData == Other.mData;
    }

    T& operator[](int index)
    {
        return mData[index];
    }

    T& operator[](int index) const
    {
        return mData[index];
    }

    std::vector<T>* operator->()
    {
        return &mData;
    }
};
