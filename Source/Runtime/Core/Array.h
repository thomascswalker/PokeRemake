#pragma once

#include <algorithm>
#include <initializer_list>
#include <string>
#include <vector>

#include "String.h"

/**
 * @brief Wrapper around std::vector to provide additional functionality.
 * @tparam T The element type.
 */
template <typename T>
class TArray
{
	using DataType = std::vector<T>;
	using RefType = T&;
	using ConstRefType = const T&;
	using IterType = typename DataType::iterator;
	using ConstIterType = typename DataType::const_iterator;

	DataType mData{};

public:
	TArray() = default;

	TArray(TArray& Other)
	{
		mData = Other.mData;
	}

	TArray(const TArray& Other)
	{
		mData = Other.mData;
	}

	TArray(const DataType& Other)
	{
		mData = Other;
	}

	TArray(const std::initializer_list<T>& InList)
	{
		mData = DataType(InList.begin(), InList.end());
	}

	~TArray() = default;

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

	void Add(const T& Value)
	{
		mData.push_back(Value);
	}

	void AddFront(const T& Value)
	{
		mData.emplace(mData.begin(), Value);
	}

	void Remove(const T& Value)
	{
		mData.erase(std::remove(mData.begin(), mData.end(), Value), mData.end());
	}

	void RemoveAt(size_t Index)
	{
		mData.erase(mData.begin() + Index);
	}

	void Clear()
	{
		mData.clear();
	}

	size_t Find(const T& Value) const
	{
		return std::find(mData.begin(), mData.end(), Value);
	}

	bool Contains(const T& Value) const
	{
		return std::find(mData.begin(), mData.end(), Value) != mData.end();
	}

	template <typename P>
	TArray Filter(const P& Predicate)
	{
		DataType Result;
		std::copy_if(mData.begin(), mData.end(), std::back_inserter(Result), Predicate);
		return Result;
	}

	template <typename P>
	void Sort(P Pred)
	{
		std::ranges::sort(mData, Pred);
	}

	template <typename P>
	TArray Sorted(P Pred)
	{
		TArray Result = *this;
		std::ranges::sort(Result, Pred);
		return Result;
	}

	std::string ToString() const
	{
		auto Result = String::Join(mData, ",");
		return std::format("[{}]", Result.c_str());
	}

	TArray& operator=(const TArray& Other)
	{
		mData = Other.mData;
		return *this;
	}

	bool operator==(const TArray& Other) const
	{
		return mData == Other.mData;
	}

	RefType operator[](int Index)
	{
		return mData[Index];
	}

	ConstRefType operator[](int Index) const
	{
		return mData[Index];
	}

	DataType* operator->()
	{
		return &mData;
	}
};
