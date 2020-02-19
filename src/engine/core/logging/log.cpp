#include "pch.hpp"
#include "log.hpp"
#include "spdlog/fmt/fmt.h"
#include <iostream>

namespace ray::logging
{

void ILog::_log(string text)
{
	std::cout << _name << ": " << text << std::endl;
}

}
