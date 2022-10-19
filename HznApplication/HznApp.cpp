#include "pch.h"

#include <HorizonEngine.h>
#include <HznEntryPoint.h>

#include "HznApp.h"




//open & save file dependencies for win32 API

#include <commdlg.h>
#include "FileManagement/ProjectFile.h"
#include "SceneManagement/Scene.h"






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
	static bool show = true;
	ImGui::ShowDemoWindow(&show);
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
	HZN_INFO("Editor Layer Attached!");
}

void EditorLayer::onRenderImgui()
{
	ImVec2 vWindowSize = ImGui::GetMainViewport()->Size;
	ImVec2 vPos0 = ImGui::GetMainViewport()->Pos;

	ImGui::SetNextWindowPos(ImVec2((float)vPos0.x, (float)vPos0.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2((float)vWindowSize.x, (float)vWindowSize.y));

	static bool my_tool_active = true;

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
					if (ImGui::MenuItem("New Scene", "Ctrl+N", false)) {

						std::string filePathfromdialog;
						Hzn::ProjectFile* p = new Hzn::ProjectFile(filePathfromdialog);
						Hzn::Scene* s = new Hzn::Scene(p);
						s->save();

					}



					if (ImGui::MenuItem("Open Scene", "Ctrl+O", false)) {

						std::string filePathfromdialog = Hzn::FileDialogs::openFile();
						
						//Check if the dtring returns empty or not
						if (filePathfromdialog != "") {

							Hzn::ProjectFile* p = new Hzn::ProjectFile(filePathfromdialog);
							Hzn::Scene* s = new Hzn::Scene(p);
							s->open();
						}
						else {
							//create new scene here
						}
					};


					ImGui::Separator();



					if (ImGui::MenuItem("Save", "Ctrl+S", false)) {


					};


					if (ImGui::MenuItem("Save As", "Ctrl+Shift+S", false)) {

						std::string filePathfromdialog = Hzn::FileDialogs::saveFile();

						//Check if the dtring returns empty or not
						if (filePathfromdialog != "") {
							Hzn::ProjectFile* p = new Hzn::ProjectFile(filePathfromdialog);
							Hzn::Scene* s = new Hzn::Scene(p);
							s->save();
						}
						else {
							//create new scene here
						}
						
					};

					ImGui::Separator();



					ImGui::MenuItem("New Project", "Ctrl+Shift+N", false);
					ImGui::MenuItem("Open Project", "Ctrl+Shift+O", false);
					ImGui::Separator();



					ImGui::MenuItem("Build Settings", NULL, false);
					ImGui::MenuItem("Build and Run", NULL, false);
					ImGui::Separator();



					//if (ImGui::MenuItem("Exit", NULL, false, pOpen != NULL))
					//*pOpen = false;
					ImGui::EndMenu();
				}
			
				if (ImGui::BeginMenu("Edit"))
				{
					if(ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Do Something */ }
					if(ImGui::MenuItem("Redo", "Ctrl+Y")) { /* Do Something */ }

					if(ImGui::MenuItem("Cut", "Ctrl+X")){ /*Do Something*/ }
					if (ImGui::MenuItem("Copy", "Ctrl+C")) { /*Do Something*/ }
					if (ImGui::MenuItem("Paste", "Ctrl+V")) { /*Do Something*/ }
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