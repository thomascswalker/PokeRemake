#include "Platform/Windows/WindowsPlatform.h"

#include <iostream>

int main()
{
	auto Platform = WindowsPlatform();
	Platform.Start();
	while (Platform.IsRunning())
	{
		std::cout << "Hello" << std::endl;
	}
}