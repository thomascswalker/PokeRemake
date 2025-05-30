#pragma once

#include <filesystem>
#include <format>
#include <iostream>
#include <string>

static std::string ColorSuffix = "\033[0m";

enum class LogLevel
{
	Debug = 10,
	Info = 20,
	Warning = 30,
	Error = 40,
};

template <typename... Types>
constexpr static void Log(std::format_string<Types...> Fmt, std::string File, LogLevel Level,
						  Types&&... Args)
{
	std::string LevelPrefix;
	std::string ColorPrefix;

	// Get file basename
	std::filesystem::path Path = File;
	// Remove file extension
	std::string FileName = Path.stem().string();
	std::string FilePrefix = std::format("[{}] ", FileName);

	switch (Level)
	{
		case LogLevel::Debug:
			LevelPrefix = "[DEBUG] ";
			ColorPrefix = "\033[37m";
			break;
		case LogLevel::Info:
			LevelPrefix = "[INFO] ";
			ColorPrefix = "\033[34m";
			break;
		case LogLevel::Warning:
			LevelPrefix = "[WARNING] ";
			ColorPrefix = "\033[33m";
			break;
		case LogLevel::Error:
			LevelPrefix = "[ERROR] ";
			ColorPrefix = "\033[31m";
			break;
		default:
			break;
	}
	std::cout << ColorPrefix << FilePrefix << LevelPrefix
			  << std::format(Fmt, std::forward<Types>(Args)...) << ColorSuffix << std::endl;
}

#define DEFINE_LOG_LEVEL(Level)                                                          \
	template <typename... Types>                                                         \
	constexpr static void Log##Level(std::format_string<Types...> Fmt, std::string File, \
									 Types&&... Args)                                    \
	{                                                                                    \
		Log(Fmt, File, LogLevel::Level, std::forward<Types>(Args)...);                   \
	}

DEFINE_LOG_LEVEL(Debug)
DEFINE_LOG_LEVEL(Info)
DEFINE_LOG_LEVEL(Warning)
DEFINE_LOG_LEVEL(Error)

#define LogDebug(x, ...) LogDebug(x, __FILE__, ##__VA_ARGS__)
#define LogInfo(x, ...) LogInfo(x, __FILE__, ##__VA_ARGS__)
#define LogWarning(x, ...) LogWarning(x, __FILE__, ##__VA_ARGS__)
#define LogError(x, ...) LogError(x, __FILE__, ##__VA_ARGS__)