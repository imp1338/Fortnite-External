#pragma once
#include "defines.h"
#include "color_console.h"

namespace ConsoleUtils 
{
	void WriteLine(std::string text)
	{
		std::cout << E("  [");
		std::cout << dye::blue(E("*").decrypt());
		std::cout << E("] ");
		std::cout << text << std::endl;
	}
	void Write(std::string text)
	{
		std::cout << E("  [");
		std::cout << dye::blue(E("*").decrypt());
		std::cout << E("] ");
		std::cout << text;
	}
}