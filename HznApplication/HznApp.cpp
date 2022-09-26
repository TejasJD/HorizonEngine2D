#include "HznApp.h"

#include <memory>

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	return std::make_shared<HznApp>();
}