#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "nativefiledialog-extended/src/include/nfd.h"

#include "Logging.h"

inline std::vector<std::string> gResourcePaths = { "Resources\\Shaders", "Resources\\Textures",
												   "Resources\\Fonts", "Resources\\Maps" };

namespace Files
{
	inline std::filesystem::path GetRootPath()
	{
		return std::filesystem::current_path().parent_path();
	}

	inline std::string FindFile(const std::string& FileName)
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

	inline bool GetOpenFileName(std::string*											FileName,
								const std::vector<std::pair<std::string, std::string>>& Filters)
	{
		NFD_Init();
		std::vector<nfdu8filteritem_t> FilterItems;
		for (const auto& Filter : Filters)
		{
			nfdu8filteritem_t Item = { Filter.first.c_str(), Filter.second.c_str() };
			FilterItems.push_back(Item);
		}
		nfdopendialogu8args_t Args = { nullptr };
		Args.filterList = FilterItems.data();
		Args.filterCount = FilterItems.size();
		char* OutPath;
		int	  Result = NFD_OpenDialogU8_With(&OutPath, &Args);

		if (Result == NFD_OKAY)
		{
			*FileName = OutPath;
		}
		else
		{
			LogDebug("Failed to open file dialog: {}", NFD_GetError());
			return false;
		}
		NFD_Quit();
		return true;
	}

	inline bool GetSaveFileName(std::string*											FileName,
								const std::vector<std::pair<std::string, std::string>>& Filters)
	{
		NFD_Init();
		std::vector<nfdu8filteritem_t> FilterItems;
		for (const auto& Filter : Filters)
		{
			nfdu8filteritem_t Item = { Filter.first.c_str(), Filter.second.c_str() };
			FilterItems.push_back(Item);
		}
		nfdsavedialogu8args_t Args = { nullptr };
		Args.filterCount = FilterItems.size();
		Args.filterList = FilterItems.data();
		char* OutPath;
		int	  Result = NFD_SaveDialogU8_With(&OutPath, &Args);

		if (Result == NFD_OKAY)
		{
			*FileName = OutPath;
		}
		else
		{
			return false;
		}
		NFD_Quit();
		return true;
	}

	inline bool WriteFile(const std::string& FileName, const std::string& Data)
	{
		std::ofstream File;
		File.open(FileName.c_str());
		if (!File.is_open())
		{
			LogError("Failed to open file: {}", FileName);
			return false;
		}
		File << Data;
		File.close();

		return true;
	}

	inline bool ReadFile(const std::string& FileName, std::string& Buffer)
	{
		LogDebug("Reading file: {}", FileName.c_str());

		std::ifstream File;
		File.open(FileName.c_str(), std::ios::binary);
		if (!File.is_open())
		{
			LogError("Failed to open file: {}", FileName);
			return false;
		}
		size_t Size = std::filesystem::file_size(FileName);
		if (Size == 0)
		{
			LogError("File is empty: {}", FileName);
			return false;
		}

		Buffer.resize(Size);

		File.read(Buffer.data(), Size);
		if (!File)
		{
			LogError("Failed to read file: {}", FileName);
			return false;
		}
		File.close();

		return true;
	}

	inline std::vector<std::string> GetFilesInDirectory(const std::string& Directory)
	{
		std::vector<std::string> OutFiles;

		auto Path = GetRootPath() / std::filesystem::path(Directory);
		for (const auto& Entry : std::filesystem::directory_iterator(Path))
		{
			OutFiles.push_back(Entry.path().string());
		}

		return OutFiles;
	}
} // namespace Files