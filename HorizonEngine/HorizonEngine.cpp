#include "HorizonEngine/Window.h";

int main() {

	Window window("Game Engine", 1920, 1080);
	glClearColor(0.36f, 0.36f, 0.36f, 1.f);

	while (!window.closed()) {
		window.clear();
		window.update();
	}

	return 0;
}