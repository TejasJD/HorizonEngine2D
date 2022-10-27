#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"

Sandbox::Sandbox(const std::string& name)
	:cameraController(Hzn::OrthographicCameraController(
		(float)Hzn::App::getApp().getAppWindow().getWidth() / (float)Hzn::App::getApp().getAppWindow().getHeight(), 1.0f))
{
	/*cameraController.enableMouseDragMovement(true);
	cameraController.enableRotation(true);*/

	food = generateFood();
}

void Sandbox::onUpdate(Hzn::TimeStep deltaTime)
{
	calculateFps();
	cameraController.getCamera().setPosition(glm::vec3{ gridSize / 2 * 0.11f, gridSize / 2 * 0.11f, 0.0f });
	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	Hzn::Renderer2D::beginScene(dynamic_cast<const Hzn::OrthographicCamera&>(cameraController.getCamera()));

	// if the food goes out of the grid, generate new food.
	if (food.first >= gridSize || food.second >= gridSize)
	{
		food = generateFood();
	}

	// update snake position

	float curTime = glfwGetTime();
	if (curTime - lTime >= 0.5f)
	{
		std::pair<int, int> moveDirection = {0, 0};
		if (direction == Direction::Up) {
			moveDirection = { 0, 1 };
		}
		else if (direction == Direction::Down) {
			moveDirection = { 0, -1 };
		}
		else if (direction == Direction::Left) {
			moveDirection = { -1, 0 };
		}
		else {
			moveDirection = { 1, 0 };
		}
		if (snakeSize.back().first + moveDirection.first < 0 
			|| snakeSize.back().first + moveDirection.first >= gridSize
			|| snakeSize.back().second + moveDirection.second < 0
			|| snakeSize.back().second + moveDirection.second >= gridSize
			|| isCollidingWithItself({ snakeSize.back().first + moveDirection.first, snakeSize.back().second + moveDirection.second }))
		{
			snakeSize = { {0, 0}, {0, 1}, {0, 2} };
			direction = Direction::Up;
			food = generateFood();
		}
		else {
			snakeSize.push_back({ snakeSize.back().first + moveDirection.first, snakeSize.back().second + moveDirection.second });

			if (snakeSize.back().first == food.first && snakeSize.back().second == food.second) {
				food = generateFood();

				if (victory) {
					snakeSize = { {0, 0}, {0, 1}, {0, 2} };
					direction = Direction::Up;
					food = generateFood();
					victory = false;
				}
			}
			else {
				snakeSize.pop_front();
			}
		}

		lTime = curTime;
	}


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

	for (int i = 0; i < gridSize; ++i)
	{
		for (int j = 0; j < gridSize; ++j)
		{
			if (i == food.first && j == food.second) {
				Hzn::Renderer2D::drawQuad(glm::vec3((float)i * 0.11f, (float)j * 0.11f, 0.0f), glm::vec3(0.1f)
					, glm::vec4(foodColor, 1.0f));
			}
			else {
				bool snakeFound = false;
				for (int k = 0; k < snakeSize.size(); ++k)
				{
					if (i == snakeSize[k].first && j == snakeSize[k].second)
					{
						Hzn::Renderer2D::drawQuad(glm::vec3((float)i * 0.11f, (float)j * 0.11f, 0.0f), glm::vec3(0.1f)
							, glm::vec4(snakeColor, 1.0f));
						snakeFound = true;
						break;
					}
				}
				if (!snakeFound) Hzn::Renderer2D::drawQuad(glm::vec3((float)i * 0.11f, (float)j * 0.11f, 0.0f), glm::vec3(0.1f));
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

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Snake color", glm::value_ptr(snakeColor));
	ImGui::ColorEdit3("Food color", glm::value_ptr(foodColor));
	ImGui::SliderInt("Grid size", &gridSize, 5, 17);
	ImGui::End();
}

void Sandbox::onEvent(Hzn::Event& event)
{
	cameraController.onEvent(event);
}

bool Sandbox::isCollidingWithItself(std::pair<int, int> nextPosition) {
	for (int i = 0; i < snakeSize.size(); i++) {
		if (nextPosition.first == snakeSize.at(i).first && nextPosition.second == snakeSize.at(i).second)
			return true;
	}

	return false;
}

std::pair<int, int> Sandbox::generateFood() {
	std::vector<std::pair<int, int>> positions;
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			bool addPosition = true;
			for (int k = 0; k < snakeSize.size(); k++) {
				if (snakeSize.at(k).first == i && snakeSize.at(k).second == j)
					addPosition = false;
			}

			if (addPosition) positions.push_back({ i, j });
		}
	}

	if (positions.size() == 0) {
		victory = true;

		return { -1, -1 };
	}

	return positions.at(rand() % positions.size());
}
