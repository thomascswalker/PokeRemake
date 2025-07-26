#pragma once
#include "Context.h"
#include "Core/Logging.h"

#include <string>
#include <vector>

#define DEFAULT_GPU_MODE "NULL"

namespace ArgParser
{
	struct TArguments
	{
		SDL_WindowFlags WindowFlags;
		std::string GPUMode;
		bool IsEditor = false;
	};

	inline TArguments Parse(int argc, char** argv)
	{
		std::vector<std::pair<std::string, std::string>> RawArgs;

		for (int i = 1; i < argc; ++i)
		{
			std::string arg = argv[i];
			if (arg.size() > 1 && arg[0] == '-')
			{
				std::string key = arg.substr(1);
				std::string Value;

				if (i + 1 < argc && argv[i + 1][0] != '-')
				{
					Value = argv[++i];
				}

				RawArgs.emplace_back(key, Value);
			}
		}

		TArguments Args{};

		for (const auto& Key : RawArgs | std::views::keys)
		{
			if (Key == "e")
			{
				Args.IsEditor = true;
			}
		}

		return Args;
	}
} // namespace ArgParser
