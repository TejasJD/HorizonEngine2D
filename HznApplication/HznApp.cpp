#include "pch.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "HorizonEngine.h"
#include "HznApp.h"

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	return std::make_shared<HznApp>();
}

SampleLayer::SampleLayer(const std::string& name) : Layer(name) {}

void SampleLayer::onAttach()
{
	HZN_INFO("Sample Layer Attached!");
}

void SampleLayer::onRenderImgui()
{
	static bool my_tool_active = true;
	ImGui::Begin("From Sample Layer", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void SampleLayer::onDetach() {}

void SampleLayer::onEvent(Hzn::Event& event)
{
	if (event.GetTypeOfEvent() == Hzn::TypeOfEvent::KeyPressed)
	{
		Hzn::KeyPressedEvent& e = (Hzn::KeyPressedEvent&)event;
		auto key = (char)e.GetKeyCode();
		HZN_INFO("{0}", key);
	}
}
