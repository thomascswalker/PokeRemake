#include "Platform/Windows/WindowsPlatform.h"

#include <iostream>

int main()
{
	auto Platform = WindowsPlatform();
	while (true)
	{
		std::cout << "Hello World!" << std::endl;
	}
	return 0;
}