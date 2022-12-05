
#pragma once

#ifndef HZN_INPUT_H
#define HZN_INPUT_H

namespace Hzn
{
	//! enum class to enable cursor control
	enum class CursorMode : uint8_t
	{
		Enabled = 0,
		Disabled = 1,
		Hidden = 2
	};

	//! struct to recieve in input
	struct Input
	{

		//! checks if key pressed
		static bool keyPressed(int key);
		//! checks if mouse button pressed
		static bool mouseButtonPressed(int button);
		//! gets mouse position
		static std::pair<float, float> getMousePos();
		//! gets Mouse X axis
		static float getMouseX();
		//! gets Mouse Y axis
		static float getMouseY();
	};

}

#endif