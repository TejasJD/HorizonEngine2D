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
	public:
		SpriteSheetGenerator() {};
		~SpriteSheetGenerator() {};
		static void createSheet(const std::string folderPath);
		

	private:

		//MaxRectsBinPack::FreeRectChoiceHeuristic chooseBestHeuristic(std::vector<sf::Texture*>* rects, size_t texWidth, size_t texHeight);
		//static std::string getXMLSheet(std::vector<sf::Image> images, std::string name);


	};
}
#endif