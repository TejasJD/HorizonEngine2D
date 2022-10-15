#pragma once

#ifndef HZN_RENDER_CONTEXT_H
#define HZN_RENDER_CONTEXT_H

namespace Hzn
{
	class RenderContext
	{
	public:
		RenderContext() {}
		virtual ~RenderContext() {}

		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};
}

#endif // !HZN_RENDER_CONTEXT_H
