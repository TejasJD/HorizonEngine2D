#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"

class Window
{
public:
	Window(const std::size_t& width, const std::size_t& height, const char *const &title)
		: m_Data{width, height, title} {}
	virtual ~Window() {}

	virtual void update() = 0;
	virtual void render() = 0;
	virtual bool shouldClose() const = 0;

protected:
	virtual void init() = 0;
	virtual void destroy() = 0;
	
	struct WindowData
	{
		std::size_t width;
		std::size_t height;
		const char* title;
	};

	WindowData m_Data;
};