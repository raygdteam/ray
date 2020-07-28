#include <core/core.hpp>
#include <core/threading/critical_section.hpp>

class RAY_CORE_API Logger
{
	pstr _name;
public:
	Logger(pcstr name);
	~Logger();
	
	void Log(pcstr msg, ...);
};