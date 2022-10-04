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
	HZN_TRACE("Sample Layer Update!");
}

void SampleLayer::onEvent(Hzn::Event& event)
{
	HZN_TRACE("Sample Layer Event");
	HZN_TRACE(event.ToString());
}
