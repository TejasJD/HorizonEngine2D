#include "AssetManager.h"
#include "Audio/AudioSource.h"

using namespace Hzn;

std::map<std::string, std::string> AssetManager::spriteFormat;
std::map<std::string, std::shared_ptr<Texture2D>> AssetManager::textureStorage;
std::map<std::string, SpriteSheetIdentifier> AssetManager::spriteSheetStorage;
std::map<std::string, std::shared_ptr<AudioSource>> AssetManager::audioStorage;
std::map<std::string, std::shared_ptr<Sprite2D>> AssetManager::spriteStorage;

void AssetManager::init(const std::filesystem::path& directoryPath)
{
	// load assets

	for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
	{
		if (!entry.is_directory() && entry.path().string().find(".png") != std::string::npos ) {
			loadTexture(entry.path().string());
		}

		if (!entry.is_directory() && entry.path().parent_path().string().find("sprites") != std::string::npos && entry.path().string().find(".png") != std::string::npos)
		{

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
		}

		if (!entry.is_directory() && entry.path().string().find(".png") != std::string::npos && entry.path().string().find("sprites") != std::string::npos) {
			loadSpriteSheet(entry.path().string(), { std::stof(spriteFormat.find("width")->second), std::stof(spriteFormat.find("height")->second) });

			for (size_t i = 0; i < std::stoi(spriteFormat.find("column")->second); i++)
			{
				for (size_t j = 0; j < std::stoi(spriteFormat.find("row")->second); j++)
				{
					loadSprite(entry.path().string(), { i, j});
				}
			}
		}

		if (!entry.is_directory() && entry.path().parent_path().string().find("audios") != std::string::npos)
		{
			loadAudio(entry.path().string());
		}

	}

}

void AssetManager::destory()
{
	spriteSheetStorage.clear();
	spriteStorage.clear();
	textureStorage.clear();
	audioStorage.clear();
}

void AssetManager::loadAudio(std::string path)
{
	audioStorage[path] = AudioSource::init(path.c_str());
}




