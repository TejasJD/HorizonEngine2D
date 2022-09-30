#pragma once 

#ifndef HZN_EVENT_H
#define HZN_EVENT_H

#include "HorizonEngine/APIMacros.h"

/*
Events are currently utilising blocking.
A new iteration may involve using the observer pattern
https://refactoring.guru/design-patterns/observer
Code that could be used can be found open source:
https://github.com/dquist/EventBus
*/

#define BIT(x) (1 << (x))

namespace Hzn {

	enum class TypeOfEvent
	{
		None = 0,
		// Window Events 
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,

		// Key Events
		KeyPressed, KeyReleased, KeyTyped,

		// Mouse Events
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

		// Application Events
		AppTick, AppUpdate, AppRender
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static TypeOfEvent GetStaticType() { return TypeOfEvent::type; }\
								virtual TypeOfEvent GetTypeOfEvent() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual TypeOfEvent GetTypeOfEvent() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// F deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetTypeOfEvent() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
#endif // !HZN_APPLICATION_EVENT_H
