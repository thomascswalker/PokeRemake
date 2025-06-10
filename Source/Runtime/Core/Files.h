#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

inline std::vector<std::string> gResourcePaths = { "Resources\\Shaders", "Resources\\Textures",
												   "Resources\\Fonts", "Resources\\Maps" };

namespace Files
{
	static std::filesystem::path GetRootPath()
	{
		return std::filesystem::current_path().parent_path();
	}

	static std::string FindFile(const std::string& FileName)
	{
		for (const auto& Path : gResourcePaths)
		{
			auto FullPath = GetRootPath() / std::filesystem::path(Path) / FileName;
			if (std::filesystem::exists(FullPath))
			{
				return FullPath.string();
			}
		}
		return "";
	}
} // namespace Files