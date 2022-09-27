#pragma once
#include "pch.h"

extern std::shared_ptr<Hzn::App> Hzn::createApp();

int main()
{
	auto app = Hzn::createApp();
	app->run();
	return 0;
}