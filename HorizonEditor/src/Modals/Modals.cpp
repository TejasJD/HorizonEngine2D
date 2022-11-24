#include <pch.h>


#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "Modals.h"
#include "EditorLayer.h"
#include "AssetManagement/AssetManager.h"



	std::string Modals::projectRootFolder;


	std::shared_ptr<Hzn::Texture> Modals::folderIcon;
	std::shared_ptr<Hzn::Texture> Modals::fileIcon;

	std::filesystem::path Modals::m_CurrentDirectory;
	std::string Modals::currentScenePath;

	char Modals::projectNameBuffer[512]{};
	char Modals::directoryPathBuffer[1024]{};
	char Modals::sceneNameBuffer[256]{};

	bool Modals::request_NewProject = false;
	bool Modals::request_NewScene = false;

	bool Modals::canCreateProject = false;



		void Modals::getCenterWindow() {
			//modal view - gets main viewport, which is window itself
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		}


		void Modals::getNewProJPopup() {
			//Begin New Project - popup modal
			if (request_NewProject)
				ImGui::OpenPopup("New Project");


			if (ImGui::BeginPopupModal("New Project", &request_NewProject, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::InputText("Project Name", projectNameBuffer, 512);
				ImGui::InputText("Root Folder", directoryPathBuffer, 1024);
				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					strcpy_s(directoryPathBuffer, Hzn::FileDialogs::openFolder().c_str());
				}
				if (!canCreateProject) {
					ImGui::Text("Enter valid name / choose valid base directory / Project with the same name exists in the same directory!");
				}

				// create new project and set it as active project.
				std::string nameString = projectNameBuffer;
				std::filesystem::path directoryPath = directoryPathBuffer;

				if (!nameString.empty() && !directoryPath.empty())
				{
					if (std::filesystem::exists(directoryPathBuffer) &&
						std::filesystem::is_directory(directoryPathBuffer) &&
						!std::filesystem::exists(directoryPath.string() + "\\" + projectNameBuffer))
					{
						canCreateProject = true;
					}
					else canCreateProject = false;
				}
				else canCreateProject = false;

				if (ImGui::Button("Create", ImVec2(120, 0)))
				{
					if (canCreateProject) {
						EditorData::m_Project_Active = Hzn::ProjectManager::create(projectNameBuffer, std::filesystem::path(directoryPathBuffer));
						EditorData::s_Scene_Active = EditorData::m_Project_Active->getActiveScene();

						// clear the directory path buffers.
						memset(projectNameBuffer, '\0', sizeof(projectNameBuffer));
						memset(directoryPathBuffer, '\0', sizeof(directoryPathBuffer));
						openProject();
					}
					ImGui::CloseCurrentPopup();
					request_NewProject = false;
				}
				ImGui::EndPopup();
			}//End new project
		}
		
		



		void Modals::getNewScenePopup() {
			//Begin new scene - pop up modal
			if (request_NewScene)
				ImGui::OpenPopup("New Scene");

			if (ImGui::BeginPopupModal("New Scene", &request_NewScene, ImGuiWindowFlags_AlwaysAutoResize))
			{

				ImGui::InputText("Scene Name", sceneNameBuffer, 256);
				std::string sceneName = std::string(sceneNameBuffer);

				if (sceneName.empty()) ImGui::Text("scene name field is empty!");

				ImGui::Separator();
				if (ImGui::Button("Create", ImVec2(120, 0)))
				{
					// create new project and set it as active project.
					if (!sceneName.empty()) {
						Hzn::ProjectManager::newScene(sceneName);
						EditorData::s_Scene_Active = EditorData::m_Project_Active->getActiveScene();
						memset(sceneNameBuffer, '\0', sizeof(sceneNameBuffer));
						request_NewScene = false;
					}
				}
				ImGui::EndPopup();
			}//END New Scene
		}


		void Modals::openProject()
		{
			projectRootFolder = EditorData::m_Project_Active->getPath().parent_path().string();
			//set current path of is  project root directory
			m_CurrentDirectory = projectRootFolder;

			std::string iconPath = projectRootFolder + "\\icons";

			currentScenePath = (EditorData::s_Scene_Active ? EditorData::s_Scene_Active->getFilePath().string() : std::string());

			for (const auto& entry : std::filesystem::recursive_directory_iterator(iconPath)) {
				//create texture of directory icon
				if (!entry.is_directory() && entry.path().string().find("DirectoryIcon.png") != std::string::npos) {
					folderIcon = Hzn::Texture2D::create(entry.path().string());
				}

				//create texture of file icon
				if (!entry.is_directory() && entry.path().string().find("FileIcon.png") != std::string::npos) {

					fileIcon = Hzn::Texture2D::create(entry.path().string());
				}
			}

			Hzn::AssetManager::init(projectRootFolder);
			
			
		}