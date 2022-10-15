#pragma once

#ifndef HZNAPP_HZNAPP_H
#define HZNAPP_HZNAPP_H

// ********** Sample Layer **********

class SampleLayer : public Hzn::Layer
{
public:
	SampleLayer(const std::string& name = "Sample Layer");

	// Inherited via Layer
	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onRenderImgui() override;

	virtual void onEvent(Hzn::Event& event) override;

};

// ********** Edior Layer **********

class EditorLayer : public Hzn::Layer
{
public:
	EditorLayer(const std::string& name = "Editor Layer");

	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onRenderImgui() override;

	virtual void onEvent(Hzn::Event& event) override;
};


class HznApp : public Hzn::App
{
public:
	HznApp()
	{
		/*addLayer(new SampleLayer());*/
		/*addLayer(new EditorLayer());*/
	}
	~HznApp() {}
};

#endif // !HZNAPP_HZNAPP_H
