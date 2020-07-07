#include <core/core.hpp>

using pstr = char*;
using pcstr = const char*;

class RAY_CORE_API Logger
{
	pstr _name;
public:
	Logger(pcstr name);
	~Logger();
	
	void Log(pcstr msg);
};
