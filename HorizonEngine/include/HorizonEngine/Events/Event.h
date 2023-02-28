#pragma once 

#ifndef HZN_EVENT_H
#define HZN_EVENT_H

#include <iostream>
#include <functional>

#include "HorizonEngine/Core/Core.h"

#define BIT(x) (1 << (x))
/// <summary>
/// Events are currently utilising blocking.
/// A new iteration may involve using the observer pattern
/// https://refactoring.guru/design-patterns/observer
/// Code that could be used can be found open source :
/// https://github.com/dquist/EventBus
/// </summary>
namespace Hzn {

	/// <summary>
	/// enum class to store the type of events ie.
	/// Window Events
	/// Key Event
	/// Mouse Events
	/// Application Events
	/// </summary>
	enum class TypeOfEvent
	{
		None = 0,
		//! Window Events 
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,

		//! Key Events
		KeyPressed, KeyReleased, KeyTyped,

		//! Mouse Events
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

		//! Application Events
		AppTick, AppUpdate, AppRender
	};

	/// <summary>
	/// Set of enumerations for categorising events.
	/// </summary>
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0), // 1 0000001
		EventCategoryInput = BIT(1), // 2 0000010
		EventCategoryKeyboard = BIT(2), // 4 0000100 Key
		EventCategoryMouse = BIT(3), // 8
		EventCategoryMouseButton = BIT(4) // 16
	};

//Event class type macro defined - Get the type of event
#define EVENT_CLASS_TYPE(type) static TypeOfEvent GetStaticType() { return TypeOfEvent::type; }\
								virtual TypeOfEvent GetTypeOfEvent() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

//Event class category macro defined - Get the category of event
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	/// <summary>
	/// Event class
	/// </summary>
	class Event
	{
	
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		virtual ~Event() = default;

		//! Bool to handle events
		bool Handled = false;

		//! Getter method of type 'TypeOfEvent'
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
		//! Constructor - initializing m_Event var
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		//! F deduced by the compiler
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
