#pragma once

#ifndef HZN_ASSETMANAGER_H
#define HZN_ASSETMANAGER_H

#include "HorizonEngine.h"
#include "pch.h"
#include <Renderer/Texture.h>
#include <Renderer/Sprite.h>
#include <Audio/AudioSource.h>



namespace Hzn
{
	class AudioSource;

	struct SpriteSheetIdentifier
	{
		std::shared_ptr<Texture2D> spriteSheet;
		glm::vec2 cellSize;
	};

	class AssetManager
	{
	public:
		// Texture storage
		static std::map<std::string, std::shared_ptr<Texture2D>> textureStorage;
		// SpriteSheet storage
		static std::map<std::string, SpriteSheetIdentifier> spriteSheetStorage;
		//Audio storage
		static std::map<std::string, std::shared_ptr<AudioSource>> audioStorage;
		//SpriteStorage
		static std::map<std::string, std::shared_ptr<Sprite2D>> spriteStorage;

		static void init(const std::filesystem::path& directoryPath);
		static void destory();

		static void loadAudio(std::string path);

		static void loadSpriteSheet(std::string path, const glm::vec2& cellSize)
		{
			spriteSheetStorage[path] = { Texture2D::create(path), cellSize };
		}

		static void loadTexture(std::string path)
		{
			textureStorage[path] = Texture2D::create(path);
		}

		static void loadSprite(std::string sheetPath, const glm::vec2& pos)
		{
			std::string sheetPathPos = sheetPath + "-;" + std::to_string(pos.x) + ";" + std::to_string(pos.y);
			spriteStorage[sheetPathPos] = Sprite2D::create(spriteSheetStorage.at(sheetPath).spriteSheet, { pos.x, pos.y }, { spriteSheetStorage.at(sheetPath).cellSize.x, spriteSheetStorage.at(sheetPath).cellSize.y });
		}

		static std::shared_ptr<Texture2D> getTexture(std::string texturePath)
		{
			return textureStorage.at(texturePath);
		}

		static std::shared_ptr<Sprite2D> getSprite(std::string sheetPath, const glm::vec2& pos)
		{
			std::string sheetPathPos = sheetPath + "-;" + std::to_string(pos.x) + ";" + std::to_string(pos.y);
			return spriteStorage.at(sheetPathPos);
		}

		static std::shared_ptr<AudioSource> getAudio(std::string audioPath)
		{
			return audioStorage.at(audioPath);
		}

	private:
		static std::map<std::string, std::string> spriteFormat;
	};

}


#endif