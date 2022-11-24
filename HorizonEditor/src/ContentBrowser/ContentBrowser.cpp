#include <pch.h>
#include "ContentBrowser.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "EditorLayer.h"
#include "Modals.h"

void ContentBrowser::OnImGuiRender()
{


	ImGui::Begin("Content Browser");

	if (!Modals::projectRootFolder.empty()) {
		if (Modals::m_CurrentDirectory != std::filesystem::path(m_ProjectRootFolder))
		{
			if (ImGui::Button("<-"))
			{
				Modals::m_CurrentDirectory = Modals::m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);



		for (auto& entry : std::filesystem::directory_iterator(Modals::m_CurrentDirectory))
		{

			if (entry.path().extension().string().find("ini") != std::string::npos || entry.path().extension().string().find("hzn") != std::string::npos || entry.path().string().find("icons") != std::string::npos)
			{
				continue;
			}

			if (entry.path().extension().string().find("meta") != std::string::npos)
			{
				std::ifstream infile(entry.path().c_str(), std::ifstream::binary);
				std::string line;

				while (std::getline(infile, line)) {
					std::istringstream is_line(line);
					std::string key;
					if (std::getline(is_line, key, ':'))
					{
						std::string value;

						if (std::getline(is_line, value))
						{
							Hzn::AssetManager::spriteFormat[key] = value;
						}
					}
				}

				std::string spriteSheetPath = entry.path().parent_path().string() + "\\" + entry.path().filename().replace_extension().string() + ".png";

				if (Hzn::AssetManager::textureStorage.find(spriteSheetPath) == Hzn::AssetManager::textureStorage.end())
				{
				
					Hzn::AssetManager::loadSpriteSheet(spriteSheetPath, 
						{ std::stof(Hzn::AssetManager::spriteFormat.find("width")->second),
							std::stof(Hzn::AssetManager::spriteFormat.find("height")->second) });

					//for (size_t i = 0; i < std::stoi(Hzn::AssetManager::spriteFormat.find("column")->second); i++)
					//{
					//	for (size_t j = 0; j < std::stoi(Hzn::AssetManager::spriteFormat.find("row")->second); j++)
					//	{
					//		Hzn::AssetManager::loadSprite(spriteSheetPath, { i, j });
					//	}
					//}

				}

				continue;
			}

			const auto& path = entry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());


			std::shared_ptr<Hzn::Texture> icon;

			if (entry.is_directory()) {
				icon = Modals::folderIcon;
			}

			else if (entry.path().string().find(".png") != std::string::npos)
			{
				icon = Hzn::AssetManager::getTexture(entry.path().string());
			}

			else
			{
				icon = Modals::fileIcon;
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			if (entry.path().parent_path().string().find("sprites") != std::string::npos)
			{

				ImGui::ImageButton((ImTextureID)icon->getId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });


				if (ImGui::Button("show sprites")) {
					m_CurrentTexturePath = entry.path().string();
				}

			}
			else {
				m_CurrentTexturePath = "";
				ImGui::ImageButton((ImTextureID)icon->getId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}

			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource()) {

				const wchar_t* filename = path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", filename, (wcslen(filename) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();

			}


			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (entry.is_directory())
					Modals::m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
	}
	ImGui::Columns(1);
	ImGui::End();

}
