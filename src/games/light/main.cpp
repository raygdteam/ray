#include <core/application.h>
#include <engine/engine.hpp>


class light_app : public ray::core::application
{
public:
	void on_startup() override
	{
		
	}

	pcstr get_resources_path() override
	{
		return "../../../build/resources";
	}
};



int WINAPI WinMain(
	HINSTANCE hInstance,     /* [input] handle to current instance */
	HINSTANCE hPrevInstance, /* [input] handle to previous instance */
	LPSTR lpCmdLine,         /* [input] pointer to command line */
	int nCmdShow             /* [input] show state of window */
)
{
	ray::engine::engine::start(new light_app);
}