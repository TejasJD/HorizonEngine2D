#include "SpriteSheetGenerator.h"


namespace Hzn
{
	MaxRectsBinPack::FreeRectChoiceHeuristic chooseBestHeuristic(std::vector<sf::Texture*>* rects, size_t texWidth, size_t texHeight) {
		MaxRectsBinPack pack;
		std::vector<Hzn::MaxRectsBinPack::FreeRectChoiceHeuristic> listHeuristics;
		listHeuristics.push_back(MaxRectsBinPack::RectBestAreaFit);
		listHeuristics.push_back(MaxRectsBinPack::RectBestLongSideFit);
		listHeuristics.push_back(MaxRectsBinPack::RectBestShortSideFit);
		listHeuristics.push_back(MaxRectsBinPack::RectBottomLeftRule);
		listHeuristics.push_back(MaxRectsBinPack::RectContactPointRule);

		MaxRectsBinPack::FreeRectChoiceHeuristic res;
		float max = 0;

		for (auto& heu : listHeuristics) {
			pack.Init(texWidth, texHeight);

			for (size_t j = 0; j < rects->size(); j++) {
				pack.Insert(rects->at(j)->getSize().x, rects->at(j)->getSize().y, heu);
			}

			if (pack.Occupancy() > max) {
				max = pack.Occupancy();
				res = heu;
			}
		}
		return res;
	}


	AssetManager SpriteSheetGenerator::createSheet(const std::string folderPath, AssetManager assetManager)
	{
		std::vector<sf::Texture*> imgTex; // images textures
		std::vector<std::string> imgTexID; // name of the images
		std::string filename = "sheet"; // filename of the sprite sheet
		sf::Vector2i size(512, 512); // size of the sprite sheet

		sf::RenderTexture rend; // texture to render the sprite sheet
		rend.create(size.x, size.y);

		Hzn::MaxRectsBinPack pack(size.x, size.y); //pack of image

		// load all the images
		for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) {

			sf::Texture* texP = new sf::Texture();
			texP->loadFromFile(entry.path().string());
			imgTex.push_back(texP);
			imgTexID.push_back(entry.path().filename().string());

		}

		float rotation = 0;

		// choose the best heuristic
		const MaxRectsBinPack::FreeRectChoiceHeuristic best1 = chooseBestHeuristic(&imgTex, 512, 512);

		for (size_t i = 0; i < imgTex.size(); i++) {
			// insert the image into the pack
			Rect packedRect = pack.Insert(imgTex[i]->getSize().x, imgTex[i]->getSize().y, best1);

			if (packedRect.height <= 0) {
				std::cout << "Error: The pack is full\n";
			}

			sf::Sprite spr(*imgTex[i]); // sprite to draw on the rendertexture

			// if the image is rotated
			if (imgTex[i]->getSize().x == packedRect.height && packedRect.width != packedRect.height) {
				rotation = 90; // set the rotation for the xml data

				// rotate the sprite to draw
				size_t oldHeight = spr.getTextureRect().height;
				spr.setPosition((float)packedRect.x, (float)packedRect.y);
				spr.rotate(rotation);
				spr.setPosition(spr.getPosition().x + oldHeight, spr.getPosition().y);
			}
			else { // if there is no rotation
				rotation = 0;
				spr.setPosition((float)packedRect.x, (float)packedRect.y);
			}

			rend.draw(spr); // draw the sprite on the sprite sheet
		}

		rend.display(); // render the texture properly

		//free the memory of the images
		for (auto& tex : imgTex) {
			delete(tex);
		}

		// save the sprite sheet
		sf::Texture tex = rend.getTexture();
		sf::Image img = tex.copyToImage(); // need to create an image to save a file
		std::string sheetsPath = std::filesystem::path(folderPath).parent_path().string() + "\\sheets\\";
		img.saveToFile(sheetsPath + filename + ".png");

		// see the occupancy of the packing
		std::cout << "pack1 : " << pack.Occupancy() << "%\n";


		for (const auto& entry : std::filesystem::recursive_directory_iterator(sheetsPath)) {
			assetManager.LoadTexture(entry.path().string(), entry.path().string());
			std::shared_ptr<Hzn::Texture2D> sheet = assetManager.GetTexture(entry.path().string());

			for (size_t i = 0; i < imgTex.size(); i++) {
				// insert the image into the pack
				Rect packedRect = pack.Insert(imgTex[i]->getSize().x, imgTex[i]->getSize().y, best1);
				
				sf::Sprite spr(*imgTex[i]); // sprite to draw on the rendertexture
				
				assetManager.LoadSpite(entry.path().filename().string(), sheet, { spr.getPosition().x,spr.getPosition().y }, { spr.getTextureRect().width, spr.getTextureRect().height });
			}
		}




		return assetManager;

	}
}