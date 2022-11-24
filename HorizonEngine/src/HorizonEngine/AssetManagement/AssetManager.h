#pragma once

#ifndef HZN_ASSETMANAGER_H
#define HZN_ASSETMANAGER_H

#include "HorizonEngine.h"
#include "pch.h"
#include <Renderer/Texture.h>
#include <Renderer/Sprite.h>

namespace Hzn
{
	class HZN_API AssetManager
	{
	public:
		AssetManager() {};
		~AssetManager() {
			_textures.clear();
			_sprites.clear();
			//_audios_.clear();};
		}

		void LoadTexture(std::string fileName, std::string filePath);
		std::shared_ptr<Hzn::Texture2D> GetTexture(std::string fileName);

		void LoadSpite(std::string fileName, std::shared_ptr<Hzn::Texture2D> sheet, glm::vec2 sheetPosition, glm::vec2 cellSize);
		std::vector<std::shared_ptr<Hzn::Sprite2D>> GetSprite(std::string fileName);

		//void LoadAudio(std::string fileName, std::string filePath);
		//Hzn::AudioSource GetAudio(std::string fileName);

		void UnloadTexture(std::string fileName);
		//void UnloadAudio(std::string fileName);
	private:
		//std::map<std::string, Hzn::AudioSource> _audios;
		std::map<std::string, std::shared_ptr<Hzn::Texture2D>> _textures;
		std::map<std::string, std::vector<std::shared_ptr<Hzn::Sprite2D>>> _sprites;
	};

}


#endif