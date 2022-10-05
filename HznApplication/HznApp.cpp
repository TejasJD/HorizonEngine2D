#include "pch.h"
#include "HorizonEngine.h"
#include "HznApp.h"

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	return std::make_shared<HznApp>();
}

void SampleLayer::onAttach()
{
	HZN_INFO("Sample Layer Attached!");
}

void SampleLayer::onDetach()
{
}

void SampleLayer::onUpdate()
{
	/*HZN_TRACE("Sample Layer Update!");*/

	/*if (Hzn::Input::keyPressed(Hzn::Key::Tab)) 
	{
		HZN_INFO("Tab pressed!");
	}*/
}

void SampleLayer::onEvent(Hzn::Event& event)
{
	if (event.GetTypeOfEvent() == Hzn::TypeOfEvent::KeyPressed)
	{
		Hzn::KeyPressedEvent& e = (Hzn::KeyPressedEvent&)event;
		auto key = (char)e.GetKeyCode();
		HZN_INFO("{0}", key);
	}
}
