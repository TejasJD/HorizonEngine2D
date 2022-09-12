#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmod.hpp>

#include <box2d/box2d.h>

int main() {
	// Test GLFW
	if (!glfwInit()) {
		std::cout << "Fail!" << std::endl;
	}
	else {
		std::cout << "Success!" << std::endl;
	}

	// Test imgui
	IMGUI_CHECKVERSION();

	// Test glm
	std::cout << glm::abs(-5) << std::endl;

	// Test FMOD
	const int MAX_CHANNELS = 64;
	FMOD::System *system;

	FMOD::System_Create(&system);
	system -> init(MAX_CHANNELS, FMOD_INIT_NORMAL, NULL);

	// Test Box2D
	b2Body body();

	return 0;
}