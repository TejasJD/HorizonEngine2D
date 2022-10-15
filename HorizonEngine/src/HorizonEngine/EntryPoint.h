#pragma once

extern std::shared_ptr<Hzn::App> Hzn::createApp();

int main()
{
	Hzn::Logging::Init();
	auto app = Hzn::createApp();
	app->run();
	return 0;
}