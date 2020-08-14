#include "renderer.hpp"

#include <glfw/glfw3.h>

#include <cstdlib>
#include <assert.h>

#pragma comment(lib, "opengl32.lib")

GLFWwindow* win = nullptr;

struct NormalizedVertex
{
	double x;
	double y;
};

NormalizedVertex NormalizeVertex(double x, double y)
{
	NormalizedVertex out = {};
	
	out.x = 2 * x / 1280.f - 1;
	out.y = 2 * y / 720.f - 1;

	out.y *= -1.f;

	return out;
}

void Renderer::Initialize()
{
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	win = glfwCreateWindow(1280, 720, "RAY_ENGINE", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwSwapInterval(0);
}

bool Renderer::ShouldClose()
{
	return glfwWindowShouldClose(win);
}

void Renderer::BeginFrame()
{
	glClearColor(0.0f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame()
{
	glfwSwapBuffers(win);
	glfwPollEvents();
}

void Renderer::DrawLine(double sx, double sy, double ex, double ey, Color color)
{
	NormalizedVertex start = NormalizeVertex(sx, sy);
	NormalizedVertex end = NormalizeVertex(ex, ey);

	glColor3f(color.Red, color.Green, color.Blue);
	glBegin(GL_LINES);
		glVertex2d(start.x, start.y);
		glVertex2d(end.x, end.y);
	glEnd();
}

void Renderer::DrawRect(double sx, double sy, double w, double h, Color color)
{
	double ex = sx + w;
	double ey = sy + h;
	
	NormalizedVertex start = NormalizeVertex(sx, sy);
	NormalizedVertex end = NormalizeVertex(ex, ey);

	glColor3f(color.Red, color.Green, color.Blue);
	glBegin(GL_QUADS);
		glVertex2d(start.x, start.y);
		glVertex2d(start.x, end.y);
		glVertex2d(end.x, end.y);
		glVertex2d(end.x, start.y);
	glEnd();
	glColor3i(0, 0, 0);
}

void Renderer::DrawOutline(double sx, double sy, double w, double h, Color color)
{
	double ex = sx + w;
	double ey = sy + h;

	NormalizedVertex start = NormalizeVertex(sx, sy);
	NormalizedVertex end = NormalizeVertex(ex, ey);

	glLineWidth(2.0f);
	glColor3f(color.Red, color.Green, color.Blue);
	glBegin(GL_LINES);
		glVertex2d(start.x, start.y);
		glVertex2d(start.x, end.y);

		glVertex2d(start.x, end.y);
		glVertex2d(end.x, end.y);

		glVertex2d(end.x, end.y);
		glVertex2d(end.x, start.y);

		glVertex2d(start.x, start.y);
		glVertex2d(end.x, start.y);
	glEnd();
}
