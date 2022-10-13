#pragma once

#include "imgui.h"
#include "HorizonEngine/Logging/Logging.h"

extern std::shared_ptr<Hzn::App> Hzn::createApp();

int main()
{
	Hzn::Logging::Init();
	auto app = Hzn::createApp();
	ImGui::SetCurrentContext(app->getImGuiAppContext());
	app->run();
	return 0;
}