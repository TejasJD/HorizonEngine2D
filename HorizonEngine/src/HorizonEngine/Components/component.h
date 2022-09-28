#pragma once

#ifndef _component_h
#define _component_h

namespace Hzn {
	class Component {
	private:
		virtual void awake() = 0;
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void fixedUpdate() = 0;
	};
}

#endif // !_component_h
