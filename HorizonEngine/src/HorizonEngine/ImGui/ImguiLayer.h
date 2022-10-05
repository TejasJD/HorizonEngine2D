#pragma once

#ifndef HZN_IMGUI_LAYER_H
#define HZN_IMGUI_LAYER_H

#include "HorizonEngine/Core/Core.h"

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
		bool onMouseMoved(MouseMovedEvent& event);
        bool onMousePressed(MouseButtonPressedEvent& event);
        bool onMouseReleased(MouseButtonReleasedEvent& event);
        bool onMouseScroll(MouseScrolledEvent& event);
        bool onCursorPos(MouseMovedEvent& event);
        bool onKeyPressed(KeyPressedEvent& event);
        bool onKeyRepeat(KeyPressedEvent& event);
        bool onKeyReleased(KeyReleasedEvent& event);
		bool onKeyTyped(KeyTypedEvent& event);
		bool onWindowResize(WindowResizeEvent& event);
		void setTime(float time) { m_Time = time; }

	private:
		float m_Time;

	};
}

#endif // !HZN_IMGUI_LAYER_H
