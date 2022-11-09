#pragma once

#ifndef HZN_ASSETMANAGER_H
#define HZN_ASSETMANAGER_H

#include "HorizonEngine.h"
#include "pch.h"
#include <Renderer/Texture.h>

namespace Hzn
{
	class HZN_API AssetManager
	{
	public:
		AssetManager() {};
		~AssetManager() {};

		void LoadTexture(std::string fileName, std::string filePath);
		std::shared_ptr<Hzn::Texture> GetTexture(std::string fileName);


		void LoadAudio(std::string fileName, std::string filePath);
		Hzn::AudioSource GetAudio(std::string fileName);

		void UnloadTexture(std::string fileName);
		void UnloadAudio(std::string fileName);
	private:
		std::map<std::string, Hzn::AudioSource> _audios;
		std::map<std::string, std::shared_ptr<Hzn::Texture>> _textures;
	};

}


#endif