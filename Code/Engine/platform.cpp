#include "pch.h"
#include "platform.hpp"

#ifdef RAY_PLATFORM_WIN
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#endif // RAY_PLATFORM_WIN

using Resolution = std::pair<u16, u16>;

class IPlatform
{
	friend class Platform;
public:
	virtual ~IPlatform() = 0;

	virtual void Preinit() = 0;
	virtual void Init() = 0;
	virtual void Destroy() = 0;
#ifdef RAY_PLATFORM_WIN
	virtual void SetParams(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nCmdShow) {} //Windows Only
	virtual HWND GetHWND() = 0;
	virtual LRESULT CALLBACK EventDespatcher(HWND, UINT, WPARAM, LPARAM) = 0;
#elif RAY_PLATFORM_LINUX
	virtual void SetParams(char** argc, int argv) {} //Linux Only
	virtual void EventDespatcher() = 0;
#endif //RAY_PLATFORM_WIN
	virtual ray_string GetPlatformName() = 0;
	virtual void GetSupportedResolutions() = 0;
	CPU::processor& GetProcessorInfo() { return m_proc; }

	virtual void SetCallback(EventCallback callback) = 0;

	virtual void OnEvent() = 0;
	virtual bool WindowIsOpen() = 0;

protected:
	CPU::processor m_proc;
	std::vector<Resolution> m_Resolutions;

	class IWindow
	{
	public:
		virtual ~IWindow() = 0;
		virtual void Create(ray_string Name, u16 Width, u16 Height) = 0;
		virtual void Destroy() = 0;

		ray_string& GetWindowName() { return m_WinDesc.Name; }
		u16& GetWindowWidth() { return m_WinDesc.res.first; }
		u16& GetWindowHeight() { return m_WinDesc.res.second; }

		virtual void OnEvent() = 0;

		virtual bool IsOpen() = 0;

		struct WindowDesc
		{
			ray_string Name;
			Resolution res;
		} m_WinDesc;
	};

	virtual IWindow* GetWindow() = 0;
};
IPlatform::IWindow::~IWindow() {};
IPlatform::~IPlatform() {};

class WindowsPlatform : public IPlatform
{
	friend class Platform;
public:
	~WindowsPlatform() override {}
	void Preinit() override;
	void Init() override;
	void Destroy() override;

#ifdef RAY_PLATFORM_WIN
	void SetParams(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nCmdShow) override;
	LRESULT CALLBACK EventDespatcher(HWND, UINT, WPARAM, LPARAM) override;
	HWND GetHWND() { return m_Window->m_hMainWnd; }
#endif

	ray_string GetPlatformName() override { return TEXT("Windows x64"); }
	void GetSupportedResolutions() override;
	IWindow* GetWindow() override { return m_Window; }

	void OnEvent() override;
	bool WindowIsOpen() override { return m_Window->IsOpen(); }

	void SetCallback(EventCallback callback) { m_Window->m_Callback = callback; }

protected:
	class Window : public IWindow
	{
	public:
		friend class WindowsPlatform;
		~Window() override {}

		void Create(ray_string Name, u16 Width, u16 Height) override;
		void Destroy() override;

		void OnEvent() override;

		bool IsOpen() override;

	private:
		MSG m_MSG;
		WNDCLASSEX m_WndClass;
		HWND m_hMainWnd;
		HINSTANCE m_hInstance;
		LPSTR m_lpCmdLine;
		s32 m_nCmdShow;
		EventCallback m_Callback;

	};

private:
	Window* m_Window;

};

#ifdef RAY_PLATFORM_WIN
LRESULT CALLBACK WindowsPlatform::EventDespatcher(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Event* e;
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}
#endif // RAY_PLATFORM_WIN

class LinuxPlatform : public IPlatform
{
public:
	void Preinit() override;
	void Init() override;
	void Destroy() override;

#ifdef RAY_PLATFORM_LINUX
	void SetParams(char** argc, int argv); //Linux Only
	void EventDespatcher() override {}

#endif // RAY_PLATFORM_LINUX

	ray_string GetPlatformName() override { return TEXT("Linux"); }
	void GetSupportedResolutions() override;
	IWindow* GetWindow() override { return m_Window; }

	void OnEvent() override;
	bool WindowIsOpen() override { return m_Window->IsOpen(); }

	void SetCallback(EventCallback callback) { m_Window->m_Callback = callback; }

	~LinuxPlatform() override;

protected:
	class Window : public IWindow
	{
		friend class LinuxPlatform;
	public:
		~Window() override {}

		void Create(ray_string Name, u16 Width, u16 Height) override;
		void Destroy() override;

		void OnEvent() override;

		bool IsOpen() override;

	private:
		EventCallback m_Callback;

	};

private:
	Window* m_Window;

};

void WindowsPlatform::Preinit()
{
	m_Window = new Window();
}

void WindowsPlatform::Init()
{
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	spdlog::set_level(spdlog::level::trace);

	CPU::init_processor_windows(&m_proc);
	GetSupportedResolutions();

	Resolution Res = m_Resolutions[4];
	m_Window->Create(TEXT("Ray Engine"), Res.first, Res.second);
}

void WindowsPlatform::Destroy()
{
	m_Window->Destroy();
	delete m_Window;
}

#ifdef RAY_PLATFORM_WIN
void WindowsPlatform::SetParams(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nCmdShow)
{
	m_Window->m_hInstance = hInstance;
	m_Window->m_lpCmdLine = lpCmdLine;
	m_Window->m_nCmdShow = nCmdShow;
}
#endif

void WindowsPlatform::GetSupportedResolutions()
{
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++)
	{
		m_Resolutions.emplace_back(dm.dmPelsWidth, dm.dmPelsHeight);
	}
	auto it = std::unique(m_Resolutions.begin(), m_Resolutions.end());
	m_Resolutions.erase(it, m_Resolutions.end());
}

void WindowsPlatform::OnEvent()
{
	m_Window->OnEvent();
	//other platform events...
}

void WindowsPlatform::Window::Create(ray_string Name, u16 Width, u16 Height)
{
	m_WinDesc.Name = Name;
	m_WinDesc.res.first = Width;
	m_WinDesc.res.second = Height;

	TCHAR ClassName[] = TEXT("Ray Engine Class");
	m_WndClass.cbSize = sizeof(m_WndClass);
	m_WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	m_WndClass.lpfnWndProc = WndProc;
	m_WndClass.lpszMenuName = NULL;
	m_WndClass.lpszClassName = ClassName;
	m_WndClass.cbWndExtra = NULL;
	m_WndClass.cbClsExtra = NULL;
	m_WndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	m_WndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	m_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_WndClass.hInstance = m_hInstance;

	RAY_ASSERT(RegisterClassEx(&m_WndClass), TEXT("Error register class!"))

		m_hMainWnd = CreateWindow(ClassName, Name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
			NULL, Width, Height, (HWND)NULL, NULL, HINSTANCE(m_hInstance), NULL);

	RAY_ASSERT(m_hMainWnd, TEXT("Window in not initializing!"))

		ShowWindow(m_hMainWnd, m_nCmdShow);
	UpdateWindow(m_hMainWnd);
}

void WindowsPlatform::Window::Destroy()
{
}

void WindowsPlatform::Window::OnEvent()
{
	TranslateMessage(&m_MSG);
	DispatchMessage(&m_MSG);
}

bool WindowsPlatform::Window::IsOpen()
{
	return GetMessage(&m_MSG, NULL, NULL, NULL);
}

void LinuxPlatform::Preinit()
{
	m_Window = new Window;
}

void LinuxPlatform::Init()
{
	CPU::init_processor_linux(&m_proc);
	GetSupportedResolutions();

	m_Window->Create(TEXT("Ray Engine"), 800, 600);
}

void LinuxPlatform::Destroy()
{


	m_Window->Destroy();
	delete m_Window;
}

#ifdef RAY_PLATFORM_LINUX
void LinuxPlatform::SetParams(char** argc, int argv)
{
}
#endif //RAY_PLATFORM_LINUX

void LinuxPlatform::GetSupportedResolutions()
{
}

void LinuxPlatform::OnEvent()
{
	m_Window->OnEvent();
	//other platform events...
}

void LinuxPlatform::Window::OnEvent()
{
}

bool LinuxPlatform::Window::IsOpen()
{
	return false;
}

LinuxPlatform::~LinuxPlatform()
{
}

void LinuxPlatform::Window::Create(ray_string Name, u16 Width, u16 Height)
{

}

void LinuxPlatform::Window::Destroy()
{

}

static IPlatform* s_Instance = nullptr;

#ifdef RAY_PLATFORM_WIN
void Platform::Init(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nCmdShow)
#elif RAY_PLATFORM_LINUX
void Platform::Init(char** argc, int argv)
#endif //RAY_PLATFORM_WIN
{
	RAY_ASSERT(!s_Instance, TEXT("Platform has initialized!"))

#ifdef RAY_PLATFORM_WIN
		s_Instance = new WindowsPlatform;
#elif RAY_PLATFORM_LINUX
		s_Instance = new LinuxPlatform;
#endif //RAY_PLATFORM_WIN

	s_Instance->Preinit();
#ifdef RAY_PLATFORM_WIN
	s_Instance->SetParams(hInstance, lpCmdLine, nCmdShow);
#elif RAY_PLATFORM_LINUX
	s_Instance->SetParams(argc, argv);
#endif
	s_Instance->Init();
}

void Platform::Destroy()
{
	RAY_ASSERT(s_Instance, TEXT("IPlatform instance is nullpointer"))
		s_Instance->Destroy();

	if (s_Instance)
		delete s_Instance;
}

#ifdef RAY_PLATFORM_WIN
HWND Platform::GetHWND()
{
	return s_Instance->GetHWND();
}
#endif

ray_string Platform::GetPlatformName()
{
	return s_Instance->GetPlatformName();
}

void Platform::OnEvent()
{
	s_Instance->OnEvent();
}

bool Platform::WindowIsOpen()
{
	return s_Instance->WindowIsOpen();
}

void Platform::SetCallback(EventCallback callback)
{
	s_Instance->SetCallback(callback);
}

bool Platform::HasFeature(CPU::Feature feature)
{
	return  CPU::has_feature(&(s_Instance->GetProcessorInfo()), feature);
}

u16 Platform::GetWidth()
{
	return s_Instance->GetWindow()->m_WinDesc.res.first;
}

u16 Platform::GetHeight()
{
	return s_Instance->GetWindow()->m_WinDesc.res.second;
}

#ifdef RAY_PLATFORM_WIN
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return dynamic_cast<WindowsPlatform*>(s_Instance)->EventDespatcher(hWnd, uMsg, wParam, lParam);
}
#endif // RAY_PLATFORM_WIN