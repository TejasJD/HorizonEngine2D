#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"

Sandbox::Sandbox()
	: m_AspectRatio((float)Hzn::App::getApp().getAppWindow().getWidth() / (float)Hzn::App::getApp().getAppWindow().getHeight()),
	m_CameraController(Hzn::OrthographicCameraController(m_AspectRatio, 1.0f))
{
	someSky = Hzn::Texture2D::create("assets/textures/someSky.png");
	checkerBoard = Hzn::Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox::onAttach()
{
	
}

void Sandbox::onDetach()
{
	
}

void Sandbox::onUpdate(Hzn::TimeStep deltaTime)
{
	float currentTime = glfwGetTime();
	m_FrameCounter++;
	if (currentTime - m_PreviousFrame >= 1.0f)
	{
		m_Fps = (float)m_FrameCounter;
		m_PreviousFrame = currentTime;
		m_FrameCounter = 0;
	}
	m_CameraController.onUpdate(deltaTime);
	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	Hzn::Renderer2D::beginScene((const Hzn::OrthographicCamera &)m_CameraController.getCamera());
	
	Hzn::Renderer2D::drawQuad({ (float)quads / 2 * 0.1f, (float)quads / 2 * 0.1f, 0.0f}, quadAngle, glm::vec3((float)quads * 0.13f));
	// generate gradient across the grid.
	for (int32_t i = 0; i < quads; ++i)
	{
		for (int32_t j = 0; j < quads; ++j)
		{
			// colored quads.
			Hzn::Renderer2D::drawQuad({ i * 0.11f, j * 0.11f, 0.1f }, quadAngle, glm::vec3(0.1f),
				{(float)(i + j) / quads, 0.0f, (float)(quads - (i + j)) / quads, 1.0f});
			// textured quads.
			/*Hzn::Renderer2D::drawQuad({ i * 1.11f, j * 1.11f, 1.0f }, quadAngle, glm::vec3(1.0f), someSky);*/
		}
	}
	/*Hzn::Renderer2D::drawQuad({ 0.0f, 0.0f, 1.0f }, quadAngle, glm::vec3(1.0f), someSky);*/

	Hzn::Renderer2D::endScene();
}

void Sandbox::onEvent(Hzn::Event& event)
{
	m_CameraController.onEvent(event);
}

void Sandbox::onRenderImgui()
{
	ImGui::Begin("FPS Counter");
	ImGui::Text("Frames: %.2f", m_Fps);
	ImGui::End();

	ImGui::Begin("Settings");
	ImGui::SliderInt("Grid Side", &quads, 5, 1000);
	ImGui::SliderFloat("Quad Angle:", &quadAngle, -180.0f, 180.0f);
	ImGui::End();

	ImGui::Begin("Metrics");
	ImGui::Text("Draw calls: %d", Hzn::Render2DStats::drawcalls);
	ImGui::Text("Quads: %d", quads * quads);
	ImGui::End();
}