// HorizonEngine.cpp : Defines the entry point for the application.
//

#include <memory>
#include "HorizonEngine.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int main()
{
	// Test CMake.
	std::cout << "Hello CMake." << std::endl;

	// Test GLFW
	std::cout << (glfwInit() ? "success" : "failure") << std::endl;

	// Test spdlog
	spdlog::info("Welcome to spdlog!");
	return 0;
}
