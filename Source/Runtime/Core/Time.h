#pragma once

#include <chrono>
#include <format>
#include <string>

namespace Time
{
	inline std::string GetCurrentTime()
	{
		return std::format("{:%T}", std::chrono::floor<std::chrono::nanoseconds>(std::chrono::system_clock::now()));
	}
} // namespace Time