#pragma once

#include <algorithm>
#include <map>
#include <vector>

namespace Containers
{
	template <typename T>
	void Add(std::vector<T>& Container, const T& Element)
	{
		Container.push_back(Element);
	}

	template <typename K, typename V>
	void Add(std::map<K, V>& Container, const K& Key, const V& Value)
	{
		Container[Key] = Value;
	}

	template <class C, typename T>
	void Remove(C& Container, const T& Element)
	{
		Container.erase(std::remove(Container.begin(), Container.end(), Element), Container.end());
	}

	template <class C, typename T>
	size_t Find(const C& Container, const T& Element)
	{
		return std::find(Container.begin(), Container.end(), Element);
	}

	template <class C, typename P>
	C Filter(const C& Container, const P& Predicate)
	{
		C Result;
		std::copy_if(Container.begin(), Container.end(), std::back_inserter(Result), Predicate);
		return Result;
	}

	template <typename T>
	bool Contains(const std::vector<T>& Container, const T& Element)
	{
		return std::find(Container.begin(), Container.end(), Element) != Container.end();
	}

	template <typename K, typename V>
	bool Contains(const std::map<K, V>& Container, const K& Key)
	{
		return Container.contains(Key);
	}
} // namespace Containers
