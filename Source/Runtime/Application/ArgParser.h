#pragma once
#include <string>
#include <vector>

namespace ArgParser
{
	struct TArguments
	{
		SDL_WindowFlags WindowFlags;
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

		for (const auto& [Key, Value] : RawArgs)
		{
			if (Key == "-r")
			{
				if (Value == "OpenGL")
				{
					Args.WindowFlags = SDL_WINDOW_OPENGL;
				}
				else if (Value == "Vulkan")
				{
					Args.WindowFlags = SDL_WINDOW_VULKAN;
				}
				else if (Value == "Metal")
				{
					Args.WindowFlags = SDL_WINDOW_METAL;
				}
				else
				{
					Args.WindowFlags = 0;
				}
			}
		}

		return Args;
	}
} // namespace ArgParser