#pragma once

#ifndef _component_h
#define _component_h

#include "componentType.h"

namespace Hzn {
	class Component {
	public:
		std::vector<ComponentType> componentTypes;
	private:
		virtual void awake() = 0;
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void fixedUpdate() = 0;
	};
}

#endif // !_component_h
