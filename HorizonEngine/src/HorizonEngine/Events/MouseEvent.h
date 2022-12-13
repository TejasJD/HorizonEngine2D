#pragma once 

#ifndef HZN_MOUSE_EVENT_H
#define HZN_MOUSE_EVENT_H

#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Codes/MouseCodes.h"

namespace Hzn {

	class MouseMovedEvent : public Event
	{
	public:
		/// <summary>
		/// MouseMovedEvent - constructor 
		/// takes in X Y coords
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		MouseMovedEvent(const float x, const float y)
			: m_MouseX(x), m_MouseY(y) {}
		//! Mouse coordinates
		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		//Return Mouse coords string
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		//! Call event macros
		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		/// <summary>
		/// MouseScrolledEvent - constructor
		/// Takes in X Y offsets
		/// calls the event macros and handles the events
		/// </summary>
		/// <param name="xOffset"></param>
		/// <param name="yOffset"></param>
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(const MouseCode button)
			: m_Button(button) {}

		MouseCode m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
#endif // !HZN_MOUSE_EVENT_H
