#pragma once

#include "HorizonEngine/Core/Core.h"

#ifndef HZN_IMGUI_LAYER_H
#define HZN_IMGUI_LAYER_H

namespace Hzn
{
	class HZN_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onEvent(Event& event) override;

		void setTime(float time) { m_Time = time; }

	private:
		float m_Time;

	};
}

#endif // !HZN_IMGUI_LAYER_H
