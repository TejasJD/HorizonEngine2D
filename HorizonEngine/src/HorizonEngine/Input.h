
#pragma once

#ifndef HZN_INPUT_H
#define HZN_INPUT_H

namespace Hzn
{
	enum class CursorMode : uint8_t
	{
		Enabled = 0,
		Disabled = 1,
		Hidden = 2
	};

	struct Input
	{
		static bool keyPressed(int key);
		static bool mouseButtonPressed(int button);
		static std::pair<float, float> getMousePos();
		static float getMouseX();
		static float getMouseY();
	};

}

#endif