#pragma once

#ifndef HZN_SPRITE_H
#define HZN_SPRITE_H

#include "Texture.h"

namespace Hzn
{
	/// <summary>
	/// Represents a 2D Sprite. It is essentially a small portion of Sprite Sheet / Texture Atlas.
	/// Sprite Sheet is just a special type 2D Texture, which has all the sprites that we want to render
	/// in just one file (texture). It can be interpreted as a grid of cells of fixed dimensions.
	/// The sides of rectangle that represents a sprite, are multiples of the sides of the grid cells
	/// in the sprite sheet.
	/// </summary>
	class Sprite2D
	{
	public:
		/// <summary>
		/// Create a sprite from Sprite Sheet. 
		/// </summary>
		/// <param name="spriteSheet">Sprite Sheet from which you wish to create the sprite.</param>
		/// <param name="sheetPosition">Position (x, y) of the sprite in the sheet. Bottom Left is (0, 0) and Top Right
		/// is (width of the sheet, height of the sheet). </param>
		/// <param name="cellSize">Dimensions of the grid cells of the sprite sheet.</param>
		/// <param name="spriteSize">Dimensions of the sprite.</param>
		/// <returns>Shared pointer to the 2D Sprite.</returns>
		static std::shared_ptr<Sprite2D> create(
			const std::shared_ptr<Texture2D>& spriteSheet,
			const glm::vec2& sheetPosition,
			const glm::vec2& cellSize,
			const glm::vec2& spriteSize = { 1.0f, 1.0f }
		);

		/// <summary>
		/// Get the sprite sheet used to create the sprite.
		/// </summary>
		/// <returns>Shared pointer to the sprite sheet.</returns>
		std::shared_ptr<Texture2D> getSpriteSheet() const { return m_SpriteSheet; }

		/// <summary>
		/// Get the texture coordinates of the sprite sheet. This information is used by the 2D Renderer
		/// to draw the sprite from the sprite sheet.
		/// </summary>
		const std::array<glm::vec2, 4>& getTexCoords() const{ return m_texCoords; }
	private:
		/// <summary>
		/// Creates a sprite. Private because, we want to return a shared pointer to the sprite,
		/// and this is done by the create function. Interally the create function uses this constructor
		/// to create a sprite and returns a shared pointer to this sprite.
		/// </summary>
		Sprite2D(const std::shared_ptr<Texture2D>& spriteSheet, const glm::vec2& min, const glm::vec2& max);

		std::shared_ptr<Texture2D> m_SpriteSheet;
		std::array<glm::vec2, 4> m_texCoords;
	};
}

#endif