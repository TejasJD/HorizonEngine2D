#include "pch.h"
#include "HznApp.h"

std::shared_ptr<Hzn::App> Hzn::createApp()
{
	return std::make_shared<HznApp>();
}