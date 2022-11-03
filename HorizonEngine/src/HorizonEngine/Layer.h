#pragma once

#ifndef HZN_LAYER_H
#define HZN_LAYER_H

#include "HorizonEngine/Core/TimeStep.h"
#include "HorizonEngine/Events/Event.h"

namespace Hzn
{
	/// <summary>
	/// Layer
	/// </summary>
	class Layer
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		Layer(const std::string& name = "Layer") : m_Name(name) {}
		virtual ~Layer() {}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::string& getName() { return m_Name; }
		/// <summary>
		/// 
		/// </summary>
		virtual void onAttach() {}
		virtual void onDetach() {}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void onUpdate(TimeStep deltaTime) {}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="event"></param>
		virtual void onEvent(Event& event) {}
		/// <summary>
		/// 
		/// </summary>
		virtual void onRenderImgui() {}

	protected:
		std::string m_Name;
	};
};

#endif // !HZN_LAYER_H
