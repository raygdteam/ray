#pragma once
#include <core/core.hpp>

#ifdef RAY_BUILD_RENDERER_NULL
#define RAY_RENDERER_NULL_API RAY_DLLEXPORTS
#else
#define RAY_RENDERER_NULL_API RAY_DLLIMPORT
#endif

struct Color
{
	float Red;
	float Green;
	float Blue;
};

class RAY_RENDERER_NULL_API Renderer
{
public:
	void Initialize();

	bool ShouldClose();
	void BeginFrame();
	void EndFrame();

	void DrawLine(double sx, double sy, double ex, double ey, Color color);
	void DrawRect(double sx, double sy, double w, double h, Color color);
	void DrawOutline(double sx, double sy, double w, double h, Color color);
};
