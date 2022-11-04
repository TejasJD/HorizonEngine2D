#pragma once

#ifndef HZN_COMPONENT_H
#define HZN_COMPONENT_H

#include <vector>
#include <string>
#include <any>

namespace Hzn {
	class Component {
	public:
		std::map<std::string, std::any>* values;
	public:
		virtual std::string getComponentType() = 0;
		virtual void setField(std::string k, std::any v) = 0;
		virtual std::any getField(std::string k) = 0;
		virtual std::vector<std::string>* stringify() = 0;
		virtual std::map<std::string, std::any>* getValues() {
			return nullptr;
		};
	protected:
		virtual void awake() = 0;
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void fixedUpdate() = 0;
	};
}

#endif // !HZN_COMPONENT_H