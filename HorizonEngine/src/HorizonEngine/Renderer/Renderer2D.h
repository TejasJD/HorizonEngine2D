#pragma once

#ifndef HZN_RENDERER_2D_H
#define HZN_RENDERER_2D_H

#include "Renderer.h"
#include "Texture.h"
#include "Sprite.h"
#include "HorizonEngine/Camera/Camera.h"

namespace Hzn
{
	/// <summary>
	/// Represents a 2D Renderer.
	/// </summary>
	class Renderer2D
	{
	public:
		/// <summary>
		/// Initialize the Renderer. Renderer must be initialized before you start making call to other methods
		/// of the rendering API. It is recommended to call this method in the Constructor of the Layer or the Application.
		/// </summary>
		static void init();

		/// <summary>
		/// Call this method to free the resources used by the renderer. It is advised to make sure that this method is
		/// called (Ideally in the destructor of the Application or the Layer so that the memory allocated by the Renderer is freed.
		/// </summary>
		static void destroy();

		/// <summary>
		/// Call this method to begin the scene, before you start making calls to other elements of the renderer in your
		/// Application Loop / Game Loop / 'onUpdate' function of the Layer (or such functions that sit in the run loop).
		/// This should be paired with the 'endScene' method after you have completed 
		/// all the other Render calls.
		/// </summary>
		/// <param name="camera">Orthographic Camera to view the 2D scene.</param>
		static void beginScene(const OrthographicCamera& camera);

		/// <summary>
		/// Call this method to end the scene, after you have made calls to other elements of the renderer in your
		/// Application Loop / Game Loop / 'onUpdate' function of the Layer (or such functions that sit in the run loop).
		/// This should be called anywhere after the 'beginScene' method (also beginScene is needed for the endScene method).
		/// </summary>
		static void endScene();

		/// <summary>
		/// Render a colored quadrilateral (4-sided shape) on the screen.
		/// </summary>
		/// <param name="position">Where should the shape appear on the screen (x, y). </param>
		/// <param name="size">What should be the size of the shape.</param>
		/// <param name="color">What should be the color of the shape.</param>
		static void drawQuad(const glm::vec2& position, const glm::vec3& size, const glm::vec4& color = glm::vec4(1.0f));

		/// <summary>
		/// Render a quadrilateral (4-sided shape) with color on the screen. z-coordinate can be used for
		/// positioning the rendered object behind or in front of others.
		/// </summary>
		/// <param name="position">Where should the shape appear on the screen (x, y, z).</param>
		/// <param name="size">What should be the size of the shape.</param>
		/// <param name="color">What should be the color of the shape.</param>
		static void drawQuad(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color = glm::vec4(1.0f));

		/// <summary>
		/// Render a rotated quadrilateral(4 - sided shape) with color on the screen.
		/// </summary>
		/// <param name="position">Where should the shape appear on the screen (x, y).</param>
		/// <param name="angle">Angle (in degrees) you want the shape to be rotated at.</param>
		/// <param name="size">What should be the size of the shape.</param>
		/// <param name="color">What should be the color of the shape.</param>
		static void drawQuad(const glm::vec2& position, float angle, const glm::vec3& size,const glm::vec4 color = glm::vec4(1.0f));

		/// <summary>
		/// Render a rotated quadrilateral(4 - sided shape) with color on the screen. z - coordinate can be used for
		/// positioning the rendered object behind or in front of others.
		/// </summary>
		/// <param name="position">Where should the shape appear on the screen (x, y, z).</param>
		/// <param name="angle">Angle (in degrees) you want the shape to be rotated at.</param>
		/// <param name="size">What should be the size of the shape.</param>
		/// <param name="color">What should be the color of the shape.</param>
		static void drawQuad(const glm::vec3& position, float angle, const glm::vec3& size,const glm::vec4 color = glm::vec4(1.0f));

		/// <summary>
		/// Render a quadrilateral (4-sided shape) with texture on the screen.
		/// </summary>
		/// <param name="position">Where should the shape appear on the screen (x, y).</param>
		/// <param name="size">What should be the size of the shape.</param>
		/// <param name="texture"> Which texture should be used on the shape.</param>
		static void drawQuad(const glm::vec2& position, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture);

		/// <summary>
		/// Render a quadrilateral(4 - sided shape) with texture on the screen. z - coordinate can be used for
		/// positioning the rendered object behind or in front of others.
		/// </summary>
		/// <param name="position">Where should the shape appear on the screen (x, y, z).</param>
		/// <param name="size">What should be the size of the shape.</param>
		/// <param name="texture"> Which texture should be used on the shape.</param>
		static void drawQuad(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture);

		/// <summary>
		/// Render a rotated quadrilateral(4 - sided shape) with texture on the screen.
		/// </summary>
		/// <param name="position">Where should the shape appear on the screen (x, y).</param>
		/// <param name="angle">Angle (in degrees) you want the shape to be rotated at.</param>
		/// <param name="size">What should be the size of the shape.</param>
		/// <param name="texture">Which texture should be used on the shape.</param>
		static void drawQuad(const glm::vec2& position, float angle, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture);

		/// <summary>
		/// Render a rotated quadrilateral(4 - sided shape) with texture on the screen. z - coordinate can be used for
		/// positioning the rendered object behind or in front of others.
		/// </summary>
		/// <param name="position">Where should the shape appear on the screen (x, y, z).</param>
		/// <param name="angle">Angle (in degrees) you want the shape to be rotated at.</param>
		/// <param name="size">What should be the size of the shape.</param>
		/// <param name="texture">Which texture should be used on the shape.</param>
		static void drawQuad(const glm::vec3& position, float angle, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture);

		/// <summary>
		/// Render a 2D sprite.
		/// </summary>
		/// <param name="position">Where should the sprite appear on the screen (x, y, z).</param>
		/// <param name="size">What should be the size of the sprite.</param>
		/// <param name="sprite">Sprite that you want to draw.</param>
		static void drawSprite(const glm::vec2& position, const glm::vec3& size, const std::shared_ptr<Sprite2D>& sprite);

		/// <summary>
		/// Render a 2D sprite. z - coordinate can be used for
		/// positioning the rendered object behind or in front of others.
		/// </summary>
		/// <param name="position">Where should the sprite appear on the screen (x, y, z).</param>
		/// <param name="size">What should be the size of the sprite.</param>
		/// <param name="sprite">Sprite that you want to draw.</param>
		static void drawSprite(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Sprite2D>& sprite);

		/// <summary>
		/// Render a rotated 2D sprite.
		/// </summary>
		/// <param name="position">Where should the sprite appear on the screen (x, y).</param>
		/// <param name="angle">Angle (in degrees) you want the sprite to be rotated at.</param>
		/// <param name="size">What should be the size of the sprite.</param>
		/// <param name="sprite">Sprite that you want to draw.</param>
		static void drawSprite(const glm::vec2& position, float angle, const glm::vec3& size, const std::shared_ptr<Sprite2D>& sprite);

		/// <summary>
		/// Render a rotated 2D sprite. z - coordinate can be used for
		/// positioning the rendered object behind or in front of others.
		/// </summary>
		/// <param name="position">Where should the sprite appear on the screen (x, y, z).</param>
		/// <param name="angle">Angle (in degrees) you want the sprite to be rotated at.</param>
		/// <param name="size">What should be the size of the sprite.</param>
		/// <param name="sprite">Sprite that you want to draw.</param>
		static void drawSprite(const glm::vec3& position, float angle, const glm::vec3& size, const std::shared_ptr<Sprite2D>& sprite);

		/// <summary>
		/// Check if the renderer was initialized.
		/// </summary>
		/// <returns>Returns true if the renderer was initialized else returns false.</returns>
		inline static bool isInitialized() { return m_Initialized; }


		/// <summary>
		/// starts a batch render session.
		/// </summary>
		static void beginBatch();
		/// <summary>
		/// submits a batch for drawing (makes the draw call).
		/// </summary>
		static void submitBatch();
		
		/// <summary>
		/// ends the batch rendering session (also calls submit batch, which is responsible for
		/// drawing the items added in the batch.
		/// </summary>
		static void endBatch();

		struct Stats
		{
			uint32_t quads = 0;
			uint32_t vertices = 0;
			uint32_t indices = 0;
			uint32_t draws = 0;
		};

		inline static Stats getStats() { return m_Stats; }

	private:
		/// <summary>
		/// Utility method to check if initialization of the Rendering API had happened.
		/// Call this method in every other API method to prevent other render calls from
		/// errors, undefined behavior etc.
		/// </summary>
		inline static void checkInitialized()
		{
			if (!isInitialized())
			{
				HZN_CORE_ERROR("2D Renderer not initialized!");
				HZN_CORE_ASSERT(false, "Assert: 2D Renderer not initialized!");
				throw std::runtime_error("2D Renderer not initialized!");
			}
		}

		static bool m_Initialized;
		static Stats m_Stats;
	};
}

#endif