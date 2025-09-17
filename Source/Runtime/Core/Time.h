#pragma once

#include <chrono>
#include <format>
#include <string>

namespace Time
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	inline std::string GetCurrentTime()
	{
		return std::format("{:%T}", std::chrono::floor<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()));
	}

	inline TimePoint Now() { return std::chrono::high_resolution_clock::now(); }

	inline float Delta(TimePoint A, TimePoint B)
	{
		return std::chrono::duration<float>(B - A).count();
	}
} // namespace Time