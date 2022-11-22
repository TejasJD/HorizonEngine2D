#include <pch.h>


#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "Modals.h"
#include "EditorLayer.h"



	std::string Modals::projectRootFolder;

	Hzn::AssetManager Modals::assetManager;
	std::map<std::string, std::string> Modals::spriteFormat;

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

			

			//create texture for image file and sprites from sprite sheets
			for (const auto& entry : std::filesystem::recursive_directory_iterator(projectRootFolder))
			{

				/*if (!entry.is_directory() && entry.path().parent_path().string().find("audios") != std::string::npos)
				{
					assetManager.LoadAudio(entry.path().string(), entry.path().string());
				}*/

				if (entry.path().string().find("icons") != std::string::npos)
				{
					continue;
				}


				if (!entry.is_directory() && entry.path().string().find(".png") != std::string::npos) {
					assetManager.LoadTexture(entry.path().string(), entry.path().string());
				}

				if (!entry.is_directory() && entry.path().parent_path().string().find("sprites") != std::string::npos && entry.path().string().find(".png") != std::string::npos) {

					for (const auto& metaFile : std::filesystem::recursive_directory_iterator(entry.path().parent_path())) {

						if (metaFile.path().string().find(".meta") != std::string::npos && metaFile.path().filename().string().substr(0, metaFile.path().filename().string().find(".")) == entry.path().filename().string().substr(0, entry.path().filename().string().find("."))) {
							std::ifstream infile(metaFile.path().c_str(), std::ifstream::binary);
							std::string line;

							while (std::getline(infile, line)) {
								std::istringstream is_line(line);
								std::string key;
								if (std::getline(is_line, key, ':'))
								{
									std::string value;

									if (std::getline(is_line, value))
									{
										spriteFormat[key] = value;
									}
								}
							}

						}
					}

					auto spriteSheet = Hzn::Texture2D::create(entry.path().string());

					int count = 0;
					for (size_t i = 0; i < std::stoi(spriteFormat.find("column")->second); i++)
					{
						for (size_t j = 0; j < std::stoi(spriteFormat.find("row")->second); j++)
						{
							assetManager.LoadSpite(entry.path().filename().string(), spriteSheet, { i, j }, { std::stof(spriteFormat.find("width")->second),std::stof(spriteFormat.find("height")->second) });
							std::shared_ptr<Hzn::Sprite2D> sprite = assetManager.GetSprite(entry.path().filename().string())[count];

							count++;
						}

					}

				}
			}
		}