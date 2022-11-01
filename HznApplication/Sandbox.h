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

private:
	bool isCollidingWithBoundary(std::pair<int, int> nextPosition);
	bool isCollidingWithItself(std::pair<int, int> nextPosition);
	std::pair<int, int> generateFood();
	void processSnakeInput();
	void recalculateFreePositions();
	void resetGame();

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

	std::deque<std::pair<int, int>> snake = { {0, 0}, {0, 1}, {0, 2} };
	std::map<std::pair<int, int>, uint64_t> freePositions;
	std::vector<std::pair<int, int>> freePositionsArray;

	std::pair<int, int> food = {0, 0};
	std::pair<int, int> snakeHead = { 0, 2 };

	Hzn::OrthographicCameraController cameraController;

	int gridSize = 5;
	int oldGridSize = 5;
	int score = 0;
	int maxScore = 0;
	bool victory = false;
	bool startGame = false;

	glm::vec3 snakeColor = { 0.0f, 1.0f, 0.0f };
	glm::vec3 foodColor = { 1.0f, 0.0f, 0.0f };
};

#endif