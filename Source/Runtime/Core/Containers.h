#pragma once
#include <algorithm>

namespace Containers
{
	template <typename C, typename P>
	C Filter(const C& Container, const P& Predicate)
	{
		C Result;
		std::copy_if(Container.begin(), Container.end(), std::back_inserter(Result), Predicate);
		return Result;
	}

	template <typename C, typename T>
	bool Contains(const C& Container, const T& Element)
	{
		return std::find(Container.begin(), Container.end(), Element) != Container.end();
	}
} // namespace Containers
