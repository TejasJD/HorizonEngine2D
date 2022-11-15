﻿/** @file Rect.cpp
	@author Jukka Jyl�nki

	This work is released to Public Domain, do whatever you want with it.
*/
#include <utility>

#include "Rect.h"

namespace Hzn {

	
	bool IsContainedIn(const Rect& a, const Rect& b)
	{
		return a.x >= b.x && a.y >= b.y
			&& a.x + a.width <= b.x + b.width
			&& a.y + a.height <= b.y + b.height;
	}

}