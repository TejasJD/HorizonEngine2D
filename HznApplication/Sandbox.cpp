#include "pch.h"
#include <HorizonEngine.h>
#include "Sandbox.h"

void Sandbox::onAttach()
{
	HZN_INFO("{0} layer attached!", this->getName());
}

void Sandbox::onDetach()
{
}

void Sandbox::onUpdate(Hzn::TimeStep deltaTime)
{
}

void Sandbox::onEvent(Hzn::Event& event)
{
}

void Sandbox::onRenderImgui()
{
}
