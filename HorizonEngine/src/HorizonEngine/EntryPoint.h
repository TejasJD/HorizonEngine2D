#pragma once

#include <memory>

extern std::shared_ptr<Hzn::App> Hzn::createApp();

int main()
{
	auto app = Hzn::createApp();
	app->run();
	return 0;
}