using pstr = char*;
using pcstr = const char*;

class Logger
{
	pstr _name;
public:
	Logger(pcstr name);
	void Log(pcstr msg);
};
