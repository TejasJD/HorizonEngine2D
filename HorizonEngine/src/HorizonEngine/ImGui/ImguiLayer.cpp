#include "pch.h"

#include "imgui.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_opengl3.h"


#include "HorizonEngine/Layer.h"
#include "HorizonEngine/App.h"

#include "ImguiLayer.h"

namespace Hzn
{
	ImguiLayer::ImguiLayer() : Layer("ImGuiLayer")
	{

	}

	ImguiLayer::~ImguiLayer()
	{

	}
	void ImguiLayer::onAttach()
	{
		HZN_CORE_INFO("Imgui Layer Attached!");
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImguiLayer::onDetach()
	{
	}
	void ImguiLayer::onUpdate()
	{
		/*HZN_CORE_TRACE("Imgui Layer Update!");*/
		ImGuiIO& io = ImGui::GetIO();
		App& app = App::getApp();

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 144.0f);
		io.DisplaySize = ImVec2(app.getAppWindow().getWidth(), app.getAppWindow().getHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImguiLayer::onEvent(Event& event)
	{

	}
}