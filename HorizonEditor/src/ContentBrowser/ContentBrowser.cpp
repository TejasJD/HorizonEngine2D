#include <pch.h>
#include "ContentBrowser.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "EditorLayer.h"
#include "Modals.h"

char ContentBrowser::fileNameBuffer[256]{};
bool ContentBrowser::request_NewFile = false;
bool ContentBrowser::request_NewFolder = false;
bool ContentBrowser::request_Rename = false;
std::string ContentBrowser::selected_file;
bool ContentBrowser::request_emptyError = false;
bool ContentBrowser::request_existsError = false;
bool ContentBrowser::request_delete = false;


void ContentBrowser::OnImGuiRender()
{
	bool itemWasHovered = false;
	bool emptySpaceClicked = false;

	ImGui::Begin(ICON_FA_FOLDER_OPEN " Content Browser", &EditorData::s_ShowContentBrowserPanel);

	if (!Modals::projectRootFolder.empty())
	{

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

		std::vector fileFilter{
			".ini",
			".hzn",
			".sln",
			".csproj",
			".lua",
		};

		for (auto& entry : std::filesystem::directory_iterator(Modals::m_CurrentDirectory))
		{
			if (std::find(fileFilter.begin(), fileFilter.end(), entry.path().extension().string()) != fileFilter.end())
			{
				continue;
			}
			if (entry.is_directory() && entry.path().string().find("bin") != std::string::npos || entry.path().string().find("icons") != std::string::npos || entry.path().string().find("load_target") != std::string::npos)
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

				if (Hzn::AssetManager::spriteStorage.find(spriteSheetPath) == Hzn::AssetManager::spriteStorage.end())
				{

					auto width = Hzn::AssetManager::getTexture(spriteSheetPath)->getWidth();
					auto height = Hzn::AssetManager::getTexture(spriteSheetPath)->getHeight();
					// (0, 0) to (width, height).
					// i * cellsize / width, j * cellsize / height
					for (int i = 0; i < width / std::stof(Hzn::AssetManager::spriteFormat.find("width")->second); ++i)
					{
						for (int j = 0; j < height / std::stof(Hzn::AssetManager::spriteFormat.find("height")->second); ++j)
						{
							std::string sheetPathPos = spriteSheetPath + "-;" + std::to_string(i) + ";" + std::to_string(j);
							Hzn::AssetManager::spriteStorage[sheetPathPos] = Hzn::Sprite2D::create(Hzn::AssetManager::getTexture(spriteSheetPath), { i, j }, { std::stof(Hzn::AssetManager::spriteFormat.find("width")->second) , std::stof(Hzn::AssetManager::spriteFormat.find("height")->second) });
						}
					}

				}

				continue;
			}

			const auto& path = entry.path();
			std::string filenameString = path.filename().string();

			/*ImGui::PushID(filenameString.c_str());*/


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

				ImGui::ImageButton(filenameString.c_str(), (ImTextureID)icon->getId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });


				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
					m_CurrentTexturePath = entry.path().string();
				}

			}
			else {
				m_CurrentTexturePath = "";
				ImGui::ImageButton(filenameString.c_str(), (ImTextureID)icon->getId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

				if (ImGui::IsItemHovered())
				{
					itemWasHovered = true;
					selected_file = path.string();
				}
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

			if (request_Rename && selected_file == path.string())
			{
				strcpy(fileNameBuffer, filenameString.c_str());
				ImGui::InputText(" ", fileNameBuffer, 256);
				std::string newName = std::string(fileNameBuffer);

				bool exist = false;
				for (const auto& entry : std::filesystem::directory_iterator(Modals::m_CurrentDirectory))
				{
					if (std::filesystem::exists((Modals::m_CurrentDirectory.string() + "\\" + newName)))
					{
						exist = true;
					}
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{

				}
				else if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					if (!newName.empty() && path.filename().string() == newName)
					{
						request_Rename = false;
					}
					else if (newName.empty())
					{
						request_emptyError = true;
						request_Rename = false;
					}
					else if (exist)
					{
						request_existsError = true;
						request_Rename = false;
					}
					else
					{
						std::filesystem::rename(selected_file, std::filesystem::path(selected_file).parent_path().string() + "\\" + newName);
						memset(fileNameBuffer, '\0', sizeof(fileNameBuffer));
						request_Rename = false;
					}

				}

			}
			else
			{
				ImGui::TextWrapped(filenameString.c_str());
			}

			ImGui::NextColumn();

			/*ImGui::PopID();*/
		}

		// Right click on the item
		if (itemWasHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("fileFolderPopup");
		}
		if (!itemWasHovered && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("ContentBrowserPopup");
		}

		if (ImGui::BeginPopup("fileFolderPopup")) {

			if (ImGui::MenuItem("Delete", false)) {
				request_delete = true;
			}
			if (ImGui::MenuItem("Rename", false)) {
				request_Rename = true;
			}
			ImGui::EndPopup();
		}

		//error popup - file/folder name empty
		if(request_emptyError)
			ImGui::OpenPopup("Rename error - empty");

		if (ImGui::BeginPopupModal("Rename error - empty", &request_emptyError)) {
			ImGui::SetWindowSize(ImVec2(400, 120));
			std::string text = "file or folder name field is empty!";
			auto windowWidth = ImGui::GetWindowSize().x;
			auto windowHeight = ImGui::GetWindowSize().y;
			auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
			auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);

			ImGui::Text(text.c_str());
			ImGui::EndPopup();
		}

		//error popup - file or folder name already exists
		if (request_existsError)
			ImGui::OpenPopup("Rename error - exists");

		if (ImGui::BeginPopupModal("Rename error - exists", &request_existsError)) {
			ImGui::SetWindowSize(ImVec2(400, 120));
			std::string text = "file or folder name already exists!";
			auto windowWidth = ImGui::GetWindowSize().x;
			auto windowHeight = ImGui::GetWindowSize().y;
			auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
			auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);

			ImGui::Text(text.c_str());
			ImGui::EndPopup();
		}

		//delete file or folder popup
		if (request_delete)
			ImGui::OpenPopup("delete file or folder");

		if (ImGui::BeginPopupModal("delete file or folder", &request_delete)) {
			ImGui::SetWindowSize(ImVec2(400, 170));
			std::string text = "The selected item will be deleted permanently!";
			auto windowWidth = ImGui::GetWindowSize().x;
			auto windowHeight = ImGui::GetWindowSize().y;
			auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::SetCursorPosY(windowHeight * 0.3f);

			ImGui::Text(text.c_str());

			ImGui::SetCursorPosY(windowHeight * 0.7f);
			ImGui::Separator();
			ImGui::SetCursorPosY(windowHeight * 0.8f);
			ImGui::SetCursorPosX(windowWidth * 0.17f);
			if (ImGui::Button("Cancle", ImVec2(120, 0))) {
				request_delete = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				std::filesystem::remove_all(selected_file);
				request_delete = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	ImGui::Columns(1);

	if (ImGui::BeginPopup("ContentBrowserPopup")) {
		if (ImGui::MenuItem("create new file", false)) {
			request_NewFile = true;

		}
		if (ImGui::MenuItem("create new folder", false)) {
			request_NewFolder = true;
		}

		ImGui::EndPopup();
	}
	//new file popup
	if (request_NewFile)
		ImGui::OpenPopup("NewFile");

	if (ImGui::BeginPopupModal("NewFile", &request_NewFile, ImGuiWindowFlags_AlwaysAutoResize))
	{

		ImGui::InputText("File Name", fileNameBuffer, 256);
		std::string fileName = std::string(fileNameBuffer);

		if (fileName.empty()) ImGui::Text("file name field is empty!");

		bool exist = false;
		for (const auto& entry : std::filesystem::directory_iterator(Modals::m_CurrentDirectory))
		{
			if (std::filesystem::exists((Modals::m_CurrentDirectory.string() + "\\" + fileName)))
			{
				exist = true;
			}
		}

		if (!fileName.empty() && exist)
		{
			ImGui::Text("file or folder name already exists!");
		}

		ImGui::Separator();
		if (ImGui::Button("Close", ImVec2(120, 0))) {
			request_NewFile = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Create", ImVec2(120, 0)))
		{
			// create new project and set it as active project.
			if (!fileName.empty() && !exist) {

				std::ofstream os(Modals::m_CurrentDirectory.string() + "\\" + fileName);
				os.close();
				memset(fileNameBuffer, '\0', sizeof(fileNameBuffer));
				request_NewFile = false;
			}
		}
		ImGui::EndPopup();
	}

	//new folder popup
	if (request_NewFolder)
		ImGui::OpenPopup("NewFolder");

	if (ImGui::BeginPopupModal("NewFolder", &request_NewFolder, ImGuiWindowFlags_AlwaysAutoResize))
	{

		ImGui::InputText("Folder Name", fileNameBuffer, 256);
		std::string folderName = std::string(fileNameBuffer);

		if (folderName.empty()) ImGui::Text("folder name field is empty!");

		bool exist = false;
		for (const auto& entry : std::filesystem::directory_iterator(Modals::m_CurrentDirectory))
		{
			if (std::filesystem::exists((Modals::m_CurrentDirectory.string() + "\\" + folderName)))
			{
				exist = true;
			}
		}

		if (!folderName.empty() && exist)
		{
			ImGui::Text("file or folder name already exists!");
		}

		ImGui::Separator();
		if (ImGui::Button("Close", ImVec2(120, 0))) {
			request_NewFolder = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Create", ImVec2(120, 0)))
		{
			// create new project and set it as active project.
			if (!folderName.empty() && !exist) {
				std::filesystem::create_directory(Modals::m_CurrentDirectory.string() + "\\" + folderName);
				memset(fileNameBuffer, '\0', sizeof(fileNameBuffer));
				request_NewFolder = false;

			}
		}
		ImGui::EndPopup();
	}

	ImGui::End();

}
