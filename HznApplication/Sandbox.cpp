#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"

Sandbox::Sandbox(const std::string& name)
	:cameraController(Hzn::OrthographicCameraController(
		(float)Hzn::App::getApp().getAppWindow().getWidth() / (float)Hzn::App::getApp().getAppWindow().getHeight(), 1.0f))
{
	/*cameraController.enableMouseDragMovement(true);
	cameraController.enableRotation(true);*/
}

void Sandbox::onUpdate(Hzn::TimeStep deltaTime)
{
	calculateFps();
	cameraController.onUpdate(deltaTime);
	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	Hzn::Renderer2D::beginScene(dynamic_cast<const Hzn::OrthographicCamera&>(cameraController.getCamera()));

	/*Hzn::Renderer2D::drawQuad(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f));
	Hzn::Renderer2D::drawQuad(glm::vec3(0.51f, 0.0f, 0.0f), glm::vec3(0.5f));
	Hzn::Renderer2D::drawQuad(glm::vec3(1.02f, 0.0f, 0.0f), glm::vec3(0.5f));*/

	// update snake position
	float curTime = glfwGetTime();
	if (curTime - lTime >= 1.0f)
	{
		snakeSize.pop_front();
		if (snakeSize.back().second + 1 >= 10)
		{
			snakeSize = { {0, 0}, {0, 1}, {0, 2} };
		}
		else snakeSize.push_back({ 0, snakeSize.back().second + 1 });
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

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			bool snakeFound = false;
			for (int k = 0; k < snakeSize.size(); ++k)
			{
				if (i == snakeSize[k].first && j == snakeSize[k].second)
				{
					Hzn::Renderer2D::drawQuad(glm::vec3((float)i * 0.11f, (float)j * 0.11f, 0.0f), glm::vec3(0.1f)
					, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					snakeFound = true;
					break;
				}
			}
			if(!snakeFound) Hzn::Renderer2D::drawQuad(glm::vec3((float)i * 0.11f, (float)j * 0.11f, 0.0f), glm::vec3(0.1f));
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
}

void Sandbox::onEvent(Hzn::Event& event)
{
	cameraController.onEvent(event);
}
