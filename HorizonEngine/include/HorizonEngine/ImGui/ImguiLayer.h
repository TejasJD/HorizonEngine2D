#pragma once

#ifndef HZN_IMGUI_LAYER_H
#define HZN_IMGUI_LAYER_H

#include "HorizonEngine/Core/Layer.h"
#include "HorizonEngine/Core/Timestep.h"
#include "HorizonEngine/Events/Event.h"

namespace Hzn
{
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(TimeStep ts) override;
		virtual void onEvent(Event& e) override;
		void imguiBegin();
		void imguiEnd();
		virtual void blockEvents(bool flag) { absorbEvents = flag; }
		void setDarkThemeColors();
	private:
		bool absorbEvents = true;
	};
}

#endif // !HZN_IMGUI_LAYER_H
