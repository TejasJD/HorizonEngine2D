#pragma once

#ifndef HZN_COMPONENT_H
#define HZN_COMPONENT_H

#include "componentType.h"

namespace Hzn {
	class Component {
	public:
		std::vector<ComponentType> componentTypes;
	protected:
		virtual void awake() = 0;
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void fixedUpdate() = 0;
	};
}

#endif // !HZN_COMPONENT_H
