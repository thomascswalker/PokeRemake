// ReSharper disable CppUnusedIncludeDirective

#pragma once

#include "Decl.h"

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

	template <typename T, typename D>
	T* ToRaw(const std::shared_ptr<D>& Data)
	{
		return dynamic_cast<T*>(Data.get());
	}

	template <typename T, typename D>
	std::vector<T*> ToRaw(std::vector<std::shared_ptr<D>>& Data)
	{
		std::vector<T*> Result;
		Result.reserve(Data.size());
		std::transform(Data.begin(), Data.end(), Result.begin(), ToRaw<T, D>);
		return Result;
	}

} // namespace Containers
