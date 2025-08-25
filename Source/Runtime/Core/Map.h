#pragma once

#include <algorithm>
#include <format>
#include <map>
#include <string>
#include <initializer_list>

#include "Array.h"

/**
 * @brief Wrapper around std::map to provide additional functionality.
 * @tparam KeyType The key type.
 * @tparam MappedType The mapped value type.
 */
template <typename KeyType, typename MappedType>
class TMap
{
public:
    using DataType      = std::map<KeyType, MappedType>;
    using ValueType     = std::pair<KeyType, MappedType>;
    using RefType       = MappedType&;
    using ConstRefType  = const MappedType&;
    using IterType      = typename DataType::iterator;
    using ConstIterType = typename DataType::const_iterator;

private:
    DataType mData;

public:
    TMap() = default;

    TMap(std::initializer_list<ValueType> InList)
    {
        mData = DataType(InList.begin(), InList.end());
    }

    TMap(const std::initializer_list<ValueType>& InList)
    {
        mData = DataType(InList.begin(), InList.end());
    }

    ~TMap() = default;

    TMap& operator=(const TMap&) = default;
    TMap& operator=(TMap&&)      = default;

    /* STL */

    IterType begin()
    {
        return mData.begin();
    }

    ConstIterType begin() const
    {
        return mData.begin();
    }

    IterType end()
    {
        return mData.end();
    }

    ConstIterType end() const
    {
        return mData.end();
    }

    /* Methods */

    size_t Size() const
    {
        return mData.size();
    }

    bool IsEmpty() const
    {
        return mData.empty();
    }

    RefType Add(const KeyType& Key)
    {
        mData.insert(Key);
        return mData[Key];
    }

    RefType Add(const KeyType& Key, const MappedType& Value)
    {
        mData[Key] = Value;
        return mData[Key];
    }

    void Remove(const KeyType& Key)
    {
        auto Iter = std::find(mData.begin(), mData.end(), Key);
        mData.erase(Iter);
    }

    void Clear()
    {
        mData.clear();
    }

    RefType Get(const KeyType& Key)
    {
        return mData.at(Key);
    }

    ConstRefType Get(const KeyType& Key) const
    {
        return mData.at(Key);
    }

    MappedType* At(const KeyType& Key)
    {
        return &mData.at(Key);
    }

    MappedType* At(const KeyType& Key) const
    {
        return &mData.at(Key);
    }

    size_t Find(const KeyType& Key) const
    {
        auto Iter = std::find(mData.begin(), mData.end(), Key);
        if (Iter != mData.end())
        {
            return Iter - mData.begin();
        }
        return mData.size();
    }

    bool Contains(const KeyType& Key) const
    {
        return std::find(mData.begin(), mData.end(), Key) != mData.end();
    }

    TArray<KeyType> Keys() const
    {
        TArray<KeyType> Result;
        for (const auto& Pair : mData)
        {
            Result.Add(Pair.first);
        }
        return Result;
    }

    TArray<MappedType> Values() const
    {
        TArray<MappedType> Result;
        for (const auto& Pair : mData)
        {
            Result.Add(Pair.second);
        }
        return Result;
    }

    std::string ToString() const
    {
        std::string Result;
        for (const auto Iter : mData)
        {
            Result += std::to_string(Iter.first) + ": " + std::to_string(Iter.second);
        }
        return std::format("[{}]", Result.c_str());
    }

    bool operator==(const TMap& Other) const
    {
        return mData == Other.mData;
    }

    bool operator!=(const TMap& Other) const
    {
        return !(*this == Other);
    }

    RefType operator[](const KeyType& Key)
    {
        return mData.at(Key);
    }

    ConstRefType operator[](const KeyType& Key) const
    {
        return mData.at(Key);
    }

    DataType* operator->()
    {
        return &mData;
    }
};
