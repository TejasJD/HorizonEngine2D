#include "pch.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "HorizonEngine.h"
#include "HznApp.h"
#include "Window.h"

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	return std::make_shared<HznApp>();
}

// *********** SAMPLE LAYER **********

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

// ************************************************************************

// *********** EDITOR LAYER **********
EditorLayer::EditorLayer(const std::string& name) : Layer(name) {}

void EditorLayer::onAttach()
{
	HZN_INFO("Sample Layer Attached!");
}

void EditorLayer::onRenderImgui()
{
	// ImGuiStyle& style = ImGui::GetStyle();
	// style.Colors[ImGuiCol_WindowBg] = ImVec4(255, 0, 0, 255); // RGB Values 

	// static bool my_tool_active = true;
	// ImGui::Begin("From Editor Layer", &my_tool_active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	// ImGui::SetNextWindowSize(ImVec2(50,500));

	ImVec2 vWindowSize = ImGui::GetMainViewport()->Size;
	ImVec2 vPos0 = ImGui::GetMainViewport()->Pos;

	ImGui::SetNextWindowPos(ImVec2((float)vPos0.x, (float)vPos0.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float)vWindowSize.x, (float)vWindowSize.y));

	if (ImGui::Begin(
		"From Editor Layer",
		nullptr,
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoTitleBar
	)
		)

	{
		{
			static const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
			ImGuiID dockSpace = ImGui::GetID("MainWindowDockspace");
			ImGui::DockSpace(dockSpace, ImVec2(0.0f, 0.0f), dockspaceFlags);

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open")){ /* Do Something */ }
					if (ImGui::MenuItem("Save")){ /* Do Something */ }
					if (ImGui::MenuItem("Close")){ /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if(ImGui::MenuItem("Undo")){ /* Do Something */ }
					if(ImGui::MenuItem("Redo")){ /* Do Something */ }

					if(ImGui::MenuItem("Cut")){ /*Do Something*/ }
					if (ImGui::MenuItem("Copy")) { /*Do Something*/ }
					if (ImGui::MenuItem("Paste")) { /*Do Something*/ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Assets"))
				{
					if(ImGui::MenuItem("Asset Submenu_1")){ /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Game Objects"))
				{
					if (ImGui::MenuItem("Game Object Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Components"))
				{
					if (ImGui::MenuItem("Components Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Window"))
				{
					if (ImGui::MenuItem("Window Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("Help Submenu_1")) { /* Do Something */ }
					ImGui::EndMenu();
				}
			}
			ImGui::EndMainMenuBar();
		}
	}
	ImGui::End();
}


void EditorLayer::onDetach() {}

void EditorLayer::onEvent(Hzn::Event& event)
{
	if (event.GetTypeOfEvent() == Hzn::TypeOfEvent::KeyPressed)
	{
		Hzn::KeyPressedEvent& e = (Hzn::KeyPressedEvent&)event;
		auto key = (char)e.GetKeyCode();
		HZN_INFO("{0}", key);
	}
}