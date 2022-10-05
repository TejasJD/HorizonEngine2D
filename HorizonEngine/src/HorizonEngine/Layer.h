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

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event& event) {}
		virtual void onRenderImgui() {}

	protected:
		std::string m_Name;
	};
};

#endif // !HZN_LAYER_H
