#include "pch.h"

#include "glm/glm.hpp"

#include "Sprite.h"

namespace Hzn
{
	Sprite2D::Sprite2D(const std::shared_ptr<Texture2D>& spriteSheet, const glm::vec2& min, const glm::vec2& max, const glm::vec2& cs)
		: m_SpriteSheet(spriteSheet)
	{
		m_texCoords[0] = min;
		m_texCoords[1] = { min.x, max.y };
		m_texCoords[2] = max;
		m_texCoords[3] = { max.x, min.y };

		m_cellSize[0] = { cs.x, cs.y };
	}

	std::shared_ptr<Sprite2D> Sprite2D::create(
		const std::shared_ptr<Texture2D>& spriteSheet,
		const glm::vec2& sheetPosition,
		const glm::vec2& cellSize,
		const glm::vec2& spriteSize
	)
	{
		glm::vec2 min =
		{
			(sheetPosition.x * cellSize.x) / spriteSheet->getWidth(),
			(sheetPosition.y * cellSize.y) / spriteSheet->getHeight(),
		};

		glm::vec2 max =
		{
			((sheetPosition.x + spriteSize.x) * cellSize.x) / spriteSheet->getWidth(),
			((sheetPosition.y + spriteSize.y) * cellSize.y) / spriteSheet->getHeight(),
		};

		glm::vec2 cs =
		{
			cellSize.x,
			cellSize.y
		};

		return std::shared_ptr<Sprite2D>(new Sprite2D(spriteSheet, min, max, cs));
	}
}