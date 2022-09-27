#pragma once

extern std::shared_ptr<Hzn::App> Hzn::createApp();

int main()
{
	bool engine = false;
	Hzn::Logging::Init();
	HZN_CORE_INFO("Initialized Loggging system");

	if (!engine)
	{
		HZN_CORE_ERROR("Error initializing engine ");
	}
	else
	{
		HZN_CORE_INFO("Initialized engine");
	}


	std::string application = "HoizonEngine Application";
	HZN_INFO("Hello {0}", application);


	auto app = Hzn::createApp();
	app->run();
	return 0;
}