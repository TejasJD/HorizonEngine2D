#pragma once

#ifndef HZN_TEXTURE_H
#define HZN_TEXTURE_H

namespace Hzn
{
	class Texture
	{
	public:
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void bind(unsigned int bind = 0) const = 0;
		
		virtual ~Texture() = default;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;
		static std::shared_ptr<Texture2D> create(const std::string& path);
	};
}

#endif // !HZN_TEXTURE_H
