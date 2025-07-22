#pragma once
#include <algorithm>

namespace Containers
{
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

	template <class C, typename T>
	bool Contains(const C& Container, const T& Element)
	{
		return std::find(Container.begin(), Container.end(), Element) != Container.end();
	}

	template <class C, typename T>
	void Remove(C& Container, const T& Element)
	{
		Container.erase(std::remove(Container.begin(), Container.end(), Element), Container.end());
	}
} // namespace Containers
