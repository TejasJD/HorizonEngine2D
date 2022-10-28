#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"

static std::pair<int, int>& operator+=(std::pair<int, int>& a, const std::pair<int, int>& b)
{
	a.first += b.first;
	a.second += b.second;
	return a;
}

static std::pair<int, int> operator+(const std::pair<int, int>& a, const std::pair<int, int>& b)
{
	std::pair<int, int> temp = a;
	temp += b;
	return temp;
}

static std::pair<int, int>& operator%=(std::pair<int, int>& a, const int& b)
{
	a.first %= b;
	a.second %= b;
	return a;
}

static std::pair<int, int> operator%(const std::pair<int, int>& a, const int& b)
{
	std::pair<int, int> temp = a;
	temp %= b;
	return temp;
}

Sandbox::Sandbox(const std::string& name)
	:cameraController(Hzn::OrthographicCameraController(
		(float)Hzn::App::getApp().getAppWindow().getWidth() / (float)Hzn::App::getApp().getAppWindow().getHeight(), 1.0f))
{
	recalculateFreePositions();
	food = generateFood();
}

void Sandbox::onUpdate(Hzn::TimeStep deltaTime)
{
	/*HZN_INFO("{0}, {1}", food.first, food.second);*/
	//HZN_DEBUG("{0}", freePositions.size());
	/*for (const auto& x : snake)
	{
		std::cout << "(" << x.first << ", " << x.second << ") ";
	}
	std::cout << std::endl;*/
	calculateFps();
	/*cameraController.onUpdate(deltaTime);*/
	cameraController.getCamera().setPosition(glm::vec3{ gridSize / 2 * 0.11f, gridSize / 2 * 0.11f, 0.0f });
	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	Hzn::Renderer2D::beginScene(dynamic_cast<const Hzn::OrthographicCamera&>(cameraController.getCamera()));

	if(gridSize != oldGridSize) recalculateFreePositions();

	// if the food goes out of the grid, generate new food.
	if (food.first >= gridSize || food.second >= gridSize)
	{
		food = generateFood();
	}

	// update snake position

	if (startGame)
	{
		float curTime = glfwGetTime();
		if (curTime - lTime >= 0.5f)
		{
			std::pair<int, int> moveDirection = { 0, 0 };
			if (direction == Direction::Up) moveDirection = { 0, 1 };
			else if (direction == Direction::Down) moveDirection = { 0, -1 };
			else if (direction == Direction::Left) moveDirection = { -1, 0 };
			else if (direction == Direction::Right) moveDirection = { 1, 0 };

			if (isCollidingWithItself(moveDirection))
			{
				recalculateFreePositions();
			}
			else
			{
				// move the snake
				snakeHead = ((snakeHead + moveDirection) % gridSize + std::make_pair(gridSize, gridSize)) % gridSize;
				// add the snakeHead to the snake
				snake.push_back(snakeHead);
				// get the previous free position from the map.
				auto it = freePositions.find(snakeHead);
				// free the previous free position index from the array.
				freePositions[freePositionsArray.back()] = it->second;
				swap(freePositionsArray[it->second], freePositionsArray.back());
				freePositionsArray.pop_back();
				// remove the previous free position from the map as well.
				freePositions.erase(it);

				// if snake eats food.
				if (snakeHead == food)
				{
					score++;
					maxScore = std::max(score, maxScore);
					food = generateFood();
				}
				else
				{
					// push the upcoming free position in the freePositions array.
					freePositions.emplace(snake.front(), (int)freePositions.size() - 1);
					freePositionsArray.push_back(snake.front());
					// free the position.
					// pop the snake tail.
					snake.pop_front();
				}
				snakeHead = snake.back();
			}

			lTime = curTime;
		}

		processSnakeInput();
	}

	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			if (!freePositions.count({ i, j }))
			{
				Hzn::Renderer2D::drawQuad({ i * 0.11, j * 0.11 }, glm::vec4(0.1f), glm::vec4(snakeColor, 1.0f));
			}
			else
			{
				if (i == food.first && j == food.second)
				{
					Hzn::Renderer2D::drawQuad({ i * 0.11, j * 0.11 }, glm::vec4(0.1f), glm::vec4(foodColor, 1.0f));
				}
				else Hzn::Renderer2D::drawQuad({ i * 0.11, j * 0.11}, glm::vec4(0.1f));
			}
		}
	}

	Hzn::Renderer2D::endScene();
}

void Sandbox::onAttach()
{
}

void Sandbox::onDetach()
{
}

void Sandbox::onRenderImgui()
{
	ImGui::Begin("FPS Counter");
	ImGui::Text("FPS: %.2f fps", fps);
	ImGui::End();

	ImGui::Begin("Snake Score");
	ImGui::Text("Score: %d", score);
	ImGui::Text("Best Score: %d", maxScore);
	ImGui::End();

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Snake color", glm::value_ptr(snakeColor));
	ImGui::ColorEdit3("Food color", glm::value_ptr(foodColor));
	ImGui::SliderInt("Grid size", &gridSize, 5, 17);
	ImGui::End();
	
	ImGui::Begin("Start Game");
	if (ImGui::SmallButton("Start"))
	{
		startGame = true;
	}
	else if (ImGui::SmallButton("Stop"))
	{
		recalculateFreePositions();
	}
	ImGui::End();
}

void Sandbox::onEvent(Hzn::Event& event)
{
	cameraController.onEvent(event);
}

bool Sandbox::isCollidingWithItself(std::pair<int, int> nextPosition) {
	std::pair<int, int> futurePosition = ((snakeHead + nextPosition) % gridSize + std::make_pair(gridSize, gridSize)) % gridSize;
	if (!freePositions.count(futurePosition))
	{
		HZN_DEBUG("Colided with itself!");
		return true;
	}
	return false;
}

std::pair<int, int> Sandbox::generateFood() {
	auto value = rand() % freePositionsArray.size();
	return freePositionsArray.at(value);
}

void Sandbox::processSnakeInput()
{
	if (Hzn::Input::keyPressed(Hzn::Key::Up))
	{
		direction = Direction::Up;
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Down))
	{
		direction = Direction::Down;
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Left))
	{
		direction = Direction::Left;
	}
	else if (Hzn::Input::keyPressed(Hzn::Key::Right))
	{
		direction = Direction::Right;
	}
}

void Sandbox::recalculateFreePositions()
{
	freePositions.clear();
	freePositionsArray.clear();
	HZN_DEBUG("Recalculating free positions...");
	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			freePositionsArray.push_back({ i, j });
			freePositions.emplace(std::make_pair(i, j), freePositionsArray.size() - 1);
		}
	}
	resetGame();
	for (auto& bodygrid : snake)
	{
		// retrieve the snake bodygrid position.
		auto it = freePositions.find(bodygrid);
		if (it != freePositions.end()) 
		{
			freePositions[freePositionsArray.back()] = it->second;
			// free the previous free position index from the array.
			swap(freePositionsArray[it->second], freePositionsArray.back());
			freePositionsArray.pop_back();
			freePositions.erase(it);
		}
	}
	HZN_DEBUG("free positions: {0}", freePositions.size());
	oldGridSize = gridSize;
	startGame = false;
}

void Sandbox::resetGame()
{
	snake = { {0, 0}, {0, 1}, {0, 2} };
	snakeHead = { 0, 2 };
	direction = Direction::Up;
	food = generateFood();
	score = 0;
}
