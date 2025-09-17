#pragma once

#include <format>
#include <initializer_list>
#include <map>
#include <string>

#include "Array.h"

/**
 * @brief Wrapper around std::map to provide additional functionality.
 * @tparam KeyType The key type.
 * @tparam ValueType The mapped value type.
 */
template <typename KeyType, typename ValueType>
class TMap
{
public:
	using DataType = std::map<KeyType, ValueType>;
	using PairType = std::pair<KeyType, ValueType>;
	using RefType = ValueType&;
	using ConstRefType = const ValueType&;
	using IterType = typename DataType::iterator;
	using ConstIterType = typename DataType::const_iterator;

private:
	DataType mData;

public:
	TMap() = default;

	TMap(std::initializer_list<PairType> InList)
	{
		mData = DataType(InList.begin(), InList.end());
	}

	~TMap() = default;

	TMap& operator=(const TMap&) = default;
	TMap& operator=(TMap&&) = default;

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

	RefType Emplace(const KeyType& Key, const ValueType& Value)
	{
		mData[Key] = Value;
		return mData[Key];
	}

	void Remove(const KeyType& Key)
	{
		auto Iter = TMap::Find(Key);
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

	ValueType* At(const KeyType& Key)
	{
		if (!mData.contains(Key))
		{
			return nullptr;
		}
		return &mData.at(Key);
	}

	ValueType* At(const KeyType& Key) const
	{
		if (!mData.contains(Key))
		{
			return nullptr;
		}
		return &mData.at(Key);
	}

	IterType Find(const KeyType& Key)
	{
		return mData.find(Key);
	}

	ConstIterType Find(const KeyType& Key) const
	{
		return mData.find(Key);
	}

	bool Contains(const KeyType& Key) const
	{
		return mData.contains(Key);
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

	TArray<ValueType> Values() const
	{
		TArray<ValueType> Result;
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

	ValueType& operator[](const KeyType& Key)
	{
		return mData[Key];
	}

	ValueType& operator[](KeyType&& Key)
	{
		return mData[Key];
	}

	DataType* operator->()
	{
		return &mData;
	}
};
