#include "AssetManager.h"

void Hzn::AssetManager::LoadTexture(std::string fileName, std::string filePath)
{
	std::shared_ptr<Hzn::Texture2D> texture = Hzn::Texture2D::create(filePath);

	if (texture)
	{
		this->_textures[fileName] = texture;
	}
}

std::shared_ptr<Hzn::Texture2D> Hzn::AssetManager::GetTexture(std::string fileName)
{
	return this->_textures.at(fileName);
}

void Hzn::AssetManager::LoadSpite(std::string fileName, std::shared_ptr<Hzn::Texture2D> sheet, glm::vec2 sheetPosition, glm::vec2 cellSize)
{
	std::shared_ptr<Hzn::Sprite2D> sprite = Hzn::Sprite2D::create(sheet, sheetPosition, cellSize);

	if (sprite)
	{
		this->_sprites[fileName].push_back(sprite);
	}
}

std::vector<std::shared_ptr<Hzn::Sprite2D>> Hzn::AssetManager::GetSprite(std::string fileName)
{
	return this->_sprites.at(fileName);
}

//void Hzn::AssetManager::LoadAudio(std::string fileName, std::string filePath)
//{
//	Hzn::AudioSource* audio = new Hzn::AudioSource();
//
//	if (audio)
//	{
//		audio->init(filePath.c_str());
//		this->_audios[fileName] = *audio;
//	}
//
//}
//
//Hzn::AudioSource Hzn::AssetManager::GetAudio(std::string fileName)
//{
//	return this->_audios.at(fileName);
//}

void Hzn::AssetManager::UnloadTexture(std::string fileName)
{
	this->_textures.erase(fileName);
}

//void Hzn::AssetManager::UnloadAudio(std::string fileName)
//{
//	this->_audios.erase(fileName);
//}
