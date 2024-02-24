#include "pch.h"

#include "HorizonEngine/AssetManagement/AssetManager.h"
#include "HorizonEngine/Audio/AudioSource.h"

namespace Hzn {

std::map<std::string, std::string> AssetManager::spriteFormat;
std::map<std::string, std::pair<std::shared_ptr<Texture2D>, glm::vec2>>
    AssetManager::textureStorage;
/*std::map<std::string, std::shared_ptr<AudioSource>>
 * AssetManager::audioStorage;*/
std::map<std::string, std::shared_ptr<Sprite2D>> AssetManager::spriteStorage;

bool AssetManager::init(const std::filesystem::path& directoryPath) {
  //! load assets
  for (const auto& entry :
       std::filesystem::recursive_directory_iterator(directoryPath)) {
    if (!entry.is_directory() &&
        entry.path().string().find(".png") != std::string::npos) {
      loadTexture(entry.path());
    }

    if (!entry.is_directory() &&
        entry.path().parent_path().string().find("sprites") !=
            std::string::npos &&
        entry.path().string().find(".png") != std::string::npos) {
      for (const auto& metaFile : std::filesystem::recursive_directory_iterator(
               entry.path().parent_path())) {
        if (metaFile.path().string().find(".meta") != std::string::npos &&
            metaFile.path().filename().string().substr(
                0, metaFile.path().filename().string().find(".")) ==
                entry.path().filename().string().substr(
                    0, entry.path().filename().string().find("."))) {
          std::ifstream infile(metaFile.path().c_str(), std::ifstream::binary);
          std::string line;

          while (std::getline(infile, line)) {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, ':')) {
              std::string value;

              if (std::getline(is_line, value)) {
                spriteFormat[key] = value;
              }
            }
          }
        }
      }
    }

    if (!entry.is_directory() &&
        entry.path().string().find(".png") != std::string::npos &&
        entry.path().string().find("sprites") != std::string::npos) {
      if (!spriteFormat.empty()) {
        loadSpriteSheet(entry.path(),
                        {std::stof(spriteFormat.find("width")->second),
                         std::stof(spriteFormat.find("height")->second)});
      }
    }

    /*if (!entry.is_directory() &&
    entry.path().parent_path().string().find("audios") != std::string::npos)
    {
            loadAudio(entry.path().string());
    }*/
  }
  return true;
}

void AssetManager::destroy() {
  spriteStorage.clear();
  textureStorage.clear();
  // audioStorage.clear();
}

/*void AssetManager::loadAudio(std::string path)
{
        audioStorage[path] = AudioSource::init(path.c_str());
}*/

void AssetManager::loadSpriteSheet(std::filesystem::path path,
                                   const glm::vec2& cellSize) {
  auto texture = Texture2D::create(path.string());
  textureStorage[path.filename().string()] = {texture, cellSize};
  auto width = texture->getWidth();
  auto height = texture->getHeight();
  // (0, 0) to (width, height).
  // i * cellsize / width, j * cellsize / height
  for (int i = 0; i < width / cellSize.x; ++i) {
    for (int j = 0; j < height / cellSize.y; ++j) {
      std::string sheetPathPos = path.filename().string() + "-;" +
                                 std::to_string(i) + ";" + std::to_string(j);
      spriteStorage[sheetPathPos] = Sprite2D::create(texture, {i, j}, cellSize);
    }
  }
}
}  // namespace Hzn
