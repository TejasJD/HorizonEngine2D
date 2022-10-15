#pragma once

#ifndef HZN_RENDERER_H
#define HZN_RENDERER_H

namespace Hzn
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1
	};

	class Renderer
	{
	public:
		static RendererAPI getAPI() { return m_API; }
	private:
		static RendererAPI m_API;
	};
}

#endif // !HZN_RENDERER_H
