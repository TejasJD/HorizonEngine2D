#include "AssetManager.h"

void Hzn::AssetManager::LoadTexture(std::string fileName, std::string filePath)
{
	std::shared_ptr<Hzn::Texture> texture = Hzn::Texture2D::create(filePath);

	if (texture)
	{
		this->_textures[fileName] = texture;
	}
}

std::shared_ptr<Hzn::Texture> Hzn::AssetManager::GetTexture(std::string fileName)
{
	return this->_textures.at(fileName);
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
