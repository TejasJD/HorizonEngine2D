#pragma once

#ifndef HZN_IMGUI_LAYER_H
#define HZN_IMGUI_LAYER_H

namespace Hzn
{
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();
		virtual void onAttach() override;
		virtual void onDetach() override;
		void ImguiLayer::onUpdate(TimeStep ts) override;
		virtual void onRenderImgui() override;
		void imguiBegin();
		void imguiEnd();

	private:
		float m_Time;

	};
}

#endif // !HZN_IMGUI_LAYER_H
