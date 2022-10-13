#pragma once

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

class HznApp : public Hzn::App
{
public:
	HznApp() 
	{
		addLayer(new SampleLayer());
	}
	~HznApp() {}
};