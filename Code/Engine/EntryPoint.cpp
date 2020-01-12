#include "pch.h"
#include "engine.hpp"

class test_application : public ray::core::application
{
public:
	void on_startup() override;
	pcstr get_resources_path() override;
};

void test_application::on_startup() { spdlog::info("Starting up 'test_application' version nikita"); }

pcstr test_application::get_resources_path() { return "../../../resources"; }

#ifdef RAY_PLATFORM_WIN
s32 common_main(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nCmdShow)
#elif defined(RAY_PLATFORM_LINUX)
i32 common_main(char** argc, i32 argv)
#endif // RAY_PLATFORM_WIN
{
	Platform::Init(hInstance, lpCmdLine, nCmdShow);
	ray::engine::engine::start(new test_application);
	Platform::Destroy();
	return 0;
}


#ifdef RAY_PLATFORM_WIN

// TODO: это не должно быть так!
s32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, s32 nCmdShow)
{
	s32 exit_code = common_main(hInstance, lpCmdLine, nCmdShow);
	
	return exit_code;
}

#endif // RAY_PLATFORM_WIN


#ifdef RAY_PLATFORM_LINUX

int main(char** argc, i32 argv)
{
	i32 exit_code = common_main(argc, argv);
	return exit_code;
}

#endif // RAY_PLATFORM_LINUX;

