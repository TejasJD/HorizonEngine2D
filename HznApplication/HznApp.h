#pragma once

#ifndef HZNAPP_HZNAPP_H
#define HZNAPP_HZNAPP_H
// ********** Edior Layer **********

class HznApp : public Hzn::App
{
public:
	HznApp()
	{
		/*Hzn::App::getApp().getAppWindow().setVsync(false);*/
		Hzn::Renderer2D::init();
	}
	~HznApp()
	{
		Hzn::Renderer2D::destroy();
	}
};

#endif // !HZNAPP_HZNAPP_H
