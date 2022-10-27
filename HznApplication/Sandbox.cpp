#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"

Sandbox::Sandbox(const std::string& name)
	:cameraController(Hzn::OrthographicCameraController(
		(float)Hzn::App::getApp().getAppWindow().getWidth() / (float)Hzn::App::getApp().getAppWindow().getHeight(), 1.0f))
{
	
}

void Sandbox::onUpdate(Hzn::TimeStep deltaTime)
{
	calculateFps();
	cameraController.onUpdate(deltaTime);
	Hzn::RenderCall::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Hzn::RenderCall::submitClear();

	Hzn::Renderer2D::beginScene(dynamic_cast<const Hzn::OrthographicCamera&>(cameraController.getCamera()));

	Hzn::Renderer2D::drawQuad(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	Hzn::Renderer2D::drawQuad(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.8f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

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
