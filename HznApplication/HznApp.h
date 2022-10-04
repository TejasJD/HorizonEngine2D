#pragma once

class SampleLayer : public Hzn::Layer 
{
public:
	SampleLayer(const std::string& name = "Sample Layer") : Layer(name) {}

	// Inherited via Layer
	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onUpdate() override;

	virtual void onEvent(Hzn::Event& event) override;

};

class HznApp : public Hzn::App
{
public:
	HznApp() 
	{
		addLayer(new SampleLayer());
		addOverlay(new Hzn::ImguiLayer());
	}
	~HznApp() {}
};