#pragma once

#ifndef HZNAPP_SANDBOX_H
#define HZNAPP_SANDBOX_H

enum class Direction
{
	Up = 0, Down, Left, Right
};

class Sandbox : public Hzn::Layer
{
public:
	Sandbox(const std::string& name = "Sample Layer");

	// Inherited via Layer
	virtual void onUpdate(Hzn::TimeStep timestep) override;

	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onRenderImgui() override;

	virtual void onEvent(Hzn::Event& event) override;

	bool isCollidingWithItself(std::pair<int, int> nextPosition);
	std::pair<int, int> generateFood();
private:
	void calculateFps()
	{
		currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - lastTime >= 1.0f)
		{
			fps = frameCount;
			frameCount = 0;
			lastTime = currentTime;
		}
	}
	glm::vec3 gridColor = { 0.0f, 1.0f, 0.0f };
	glm::vec3 gridPosition = { 0.0, 0.0f, 0.0f };

	float fps = 0.0f;
	float frameCount = 0.0f;
	float currentTime = 0.0f;
	float lastTime = 0.0f;
	float objScale = 1.0f;

	float lTime = 0.0f;

	Direction direction = Direction::Up;

	std::deque<std::pair<int, int>> snakeSize = { {0, 0}, {0, 1}, {0, 2} };
	std::pair<int, int> food = {0, 0};

	Hzn::OrthographicCameraController cameraController;

	int gridSize = 10;
	bool victory = false;

	glm::vec3 snakeColor = { 1.0f, 0.0f, 0.0f };
	glm::vec3 foodColor = { 0.0f, 1.0f, 0.0f };
};

#endif