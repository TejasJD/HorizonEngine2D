#pragma once

#include "HorizonEngine/Window.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

// Implementation of the Window class with OpenGL as the rendering API
class MSWindow : public Window
{
public:
	// Constructor creates window and initializes the object
	MSWindow(const std::size_t& width, const std::size_t& height, const char* const& title);


	virtual ~MSWindow();

	// declarations of virtual methods of the window class
	virtual void update() override;
	virtual void render() override;
	virtual bool shouldClose() const override;

private:
	virtual void init() override;
	virtual void destroy() override;

	GLFWwindow* m_Window;
};