#pragma once

#ifndef HZN_TEXTURE_H
#define HZN_TEXTURE_H

#include <cstdint>

#include "HorizonEngine/Core/Core.h"

namespace Hzn
{
	/// <summary>
	/// Abstract class for Textures.
	/// </summary>
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual bool operator==(const Texture& rhs) const = 0;
		virtual bool operator!=(const Texture& rhs) const = 0;
		
		/// <summary>
		/// Get the width of the texture.
		/// </summary>
		virtual uint32_t getWidth() const = 0;
		
		/// <summary>
		/// Get the height of the texture.
		/// </summary>
		virtual uint32_t getHeight() const = 0;
		
		/// <summary>
		/// bind the texture
		/// </summary>
		/// <param name="slot"> Bind the texture to one of the texture units {0, 32} </param>
		virtual void bind(uint32_t slot = 0) const = 0;

		/// <summary>
		/// unbind the texture.
		/// </summary>
		virtual void unbind() const = 0;

		/// <summary>
		/// Upload data to texture.
		/// </summary>
		/// <param name="data"> Data to be uploaded to the texture.</param>
		/// <param name="size"> Size of the data in bytes.</param>
		virtual void setData(void *data, uint32_t size) = 0;
		
		/// <summary>
		/// get the texture ID.
		/// </summary>
		virtual uint32_t getId() const = 0;


		/// <summary>
		/// get the texture path.
		/// </summary>
		virtual const std::string& getPath() const = 0;
	};

	/// <summary>
	/// Represents 2D Textures.
	/// </summary>
	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;
		
		/// <summary>
		///	Create an object of Texture2D class (by uploading image data).
		/// </summary>
		/// <param name="path"> Provide the file-path of to the image data to build the texture from.</param>
		static std::shared_ptr<Texture2D> create(const std::string& path);

		/// <summary>
		/// Create an object of Texture2D class of specific width and height. Holds no data.
		/// It needs to be set later in the program.
		/// </summary>
		/// <param name="width"> Width of the texture.</param>
		/// <param name="height"> Height of the texture.</param>
		static std::shared_ptr<Texture2D> create(uint32_t width, uint32_t height);
	};
}

#endif // !HZN_TEXTURE_H
