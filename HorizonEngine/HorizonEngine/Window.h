#pragma once

#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class Window {
public:
	ImFont* widgetsTitleFont;
	ImFont* buttonsFont;
private:
	const char* title;
	int width, height;
	GLFWwindow* window;
	bool isClosed;
public:
	Window(const char* title, int width, int height);
	~Window();
	bool closed() const;
	void clear() const;
	void update();

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
private:
	bool init();
};