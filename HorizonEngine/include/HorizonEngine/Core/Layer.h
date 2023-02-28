#pragma once

#ifndef HZN_LAYER_H
#define HZN_LAYER_H

#include <string>

#include "HorizonEngine/Core/TimeStep.h"
#include "HorizonEngine/Events/Event.h"

namespace Hzn
{
	/// <summary>
	/// Layer class
	/// </summary>
	class Layer
	{
	public:
		/// <summary>
		/// Layer class constructor, set name="Layer" & initialises it.
		/// Followed by Layer deconstructor.
		/// </summary>
		/// <param name="name"></param>
		Layer(const char* name = "Layer") : m_Name(name) {}
		virtual ~Layer() {}
		/// <summary>
		/// Name getter
		/// </summary>
		/// <returns></returns>
		const std::string& getName() { return m_Name; }
		/// <summary>
		/// onAttach() & onDetach() methods declaration
		/// </summary>
		virtual void onAttach() {}
		virtual void onDetach() {}
		/// <summary>
		/// The onUpdate method uses delta time to calculate to elapsed since the last frame
		/// And is used for updating scenery.
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void onUpdate(TimeStep deltaTime) {}
		/// <summary>
		/// Calls the event method.
		/// </summary>
		/// <param name="event"></param>
		virtual void onEvent(Event& event) {}
		/// <summary>
		/// Renders the ImGUI() interface
		/// </summary>
		virtual void onRenderImgui() {}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="flag"></param>
		virtual void blockEvents(bool flag) {};

	protected:
		std::string m_Name;
	};
};

#endif // !HZN_LAYER_H
