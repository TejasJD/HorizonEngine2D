#pragma once

#ifndef HZN_SPRITESHEETGENERATOR_H
#define HZN_SPRITESHEETGENERATOR_H

#include "HorizonEngine.h"
#include "pch.h"
#include "SFML/Graphics.hpp"


namespace Hzn {

	class MaxRectsBinPack;
	class SpriteSheetGenerator {
		friend class MaxRectsBinPack;
		friend class AssetManager;
	public:
		SpriteSheetGenerator() {};
		~SpriteSheetGenerator() {};
		static AssetManager createSheet(const std::string folderPath, AssetManager assetManager);
	};
}
#endif