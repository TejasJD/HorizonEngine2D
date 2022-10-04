#pragma once

#ifndef HZN_LAYER_H
#define HZN_LAYER_H

#include "HorizonEngine/Core/Core.h"
#include "HorizonEngine/Events/Event.h"

namespace Hzn
{
	class HZN_API Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_Name(name) {}
		virtual ~Layer() {}

		inline const std::string& getName() { return m_Name; }

		virtual void onAttach() = 0;
		virtual void onDetach() = 0;
		virtual void onUpdate() = 0;
		virtual void onEvent(Event& event) = 0;

	protected:
		std::string m_Name;
	};
};

#endif // !HZN_LAYER_H
