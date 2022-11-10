#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"

Sandbox::Sandbox()
	: m_AspectRatio(static_cast<float>(Hzn::App::getApp().getAppWindow().getWidth()) / Hzn::App::getApp().getAppWindow().getHeight()),
	m_CameraController(Hzn::OrthographicCameraController(m_AspectRatio, 1.0f))
{
	m_Bear = Hzn::Texture2D::create("assets/textures/bear.png");
	m_SpriteSheet = Hzn::Texture2D::create("assets/sample_game_assets/Tilemap/tilemap_packed.png");
	m_Sprite = Hzn::Sprite2D::create(m_SpriteSheet, { 0.0f, 0.0f }, {16.0f, 16.0f}, { 1, 1 });
}

void Sandbox::onAttach()
{
	
}

void Sandbox::onDetach()
{
	
}

void Sandbox::onUpdate(Hzn::TimeStep deltaTime)
{
	const auto currentTime = static_cast<float>(glfwGetTime());
	m_FrameCounter++;
	if (currentTime - m_PreviousFrame >= 1.0f)
	{
		m_Fps = static_cast<const float>(m_FrameCounter);
		m_PreviousFrame = currentTime;
		m_FrameCounter = 0;
	}
	m_CameraController.onUpdate(deltaTime);
	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	Hzn::Renderer2D::beginScene(dynamic_cast<const Hzn::OrthographicCamera &>(m_CameraController.getCamera()));
	// generate gradient across the grid.
	//for (int32_t i = 0; i < quads; ++i)
	//{
	//	for (int32_t j = 0; j < quads; ++j)
	//	{
	//		// colored quads.
	//		const glm::vec4 color = glm::lerp(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), (float)(i + j) / (2 * quads));
	//		Hzn::Renderer2D::drawQuad({ i * 0.11f, j * 0.11f, 0.0f }, quadAngle, glm::vec3(0.1f), color);
	//		// textured quads.
	//		/*Hzn::Renderer2D::drawQuad({ i * 1.11f, j * 1.11f, 1.0f }, quadAngle, glm::vec3(1.0f), someSky);*/
	//	}
	//}
	Hzn::Renderer2D::drawSprite({ 0.0f, 0.0f, 0.0f }, glm::vec3(1.0f), m_Sprite);

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
	/*ImGui::Image((ImTextureID)m_Bear->getId(), { 50.0f, 50.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });*/
	auto texCoords = m_Sprite->getTexCoords();
	ImGui::Image((ImTextureID)m_SpriteSheet->getId(), { 160.0f, 160.0f }, {texCoords[0].x, texCoords[2].y},
		{texCoords[2].x, texCoords[0].y});
	ImGui::End();

	const Hzn::Renderer2DStats& stats = Hzn::Renderer2D::getStats();

	ImGui::Begin("Metrics");
	ImGui::Text("Draw calls: %d", stats.draws);
	ImGui::Text("Quads: %d", stats.quads);
	ImGui::Text("Vertices: %d", stats.vertices);
	ImGui::Text("Indices: %d", stats.indices);
	ImGui::End();
}