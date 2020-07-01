using pstr = char*;
using pcstr = const char*;

#if _DEBUG
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

class DLLEXPORT Logger
{
	pstr _name;
public:
	Logger(pcstr name);
	void Log(pcstr msg);
};
