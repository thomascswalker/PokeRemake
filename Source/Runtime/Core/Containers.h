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
} // namespace Containers
