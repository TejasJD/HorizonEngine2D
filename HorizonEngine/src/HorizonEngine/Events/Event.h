#pragma once 

#ifndef HZN_EVENT_H
#define HZN_EVENT_H
/*
Events are currently utilising blocking.
A new iteration may involve using the observer pattern
https://refactoring.guru/design-patterns/observer
Code that could be used can be found open source:
https://github.com/dquist/EventBus
*/

#include "HorizonEngine/Core/Core.h"
#include "fmt/ostream.h"

#define BIT(x) (1 << (x))

namespace Hzn {


	enum class HZN_API TypeOfEvent
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

	enum HZN_API EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0), // 1 0000001
		EventCategoryInput = BIT(1), // 2 0000010
		EventCategoryKeyboard = BIT(2), // 4 0000100 Key
		EventCategoryMouse = BIT(3), // 8
		EventCategoryMouseButton = BIT(4) // 16
	};

#define EVENT_CLASS_TYPE(type) static TypeOfEvent GetStaticType() { return TypeOfEvent::type; }\
								virtual TypeOfEvent GetTypeOfEvent() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HZN_API Event
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

	class HZN_API EventDispatcher
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

	HZN_API inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}

template<>
struct fmt::formatter<Hzn::Event> : fmt::ostream_formatter {};
#endif // !HZN_APPLICATION_EVENT_H
