#pragma once

#ifndef HZNAPP_SANDBOX_H
#define HZNAPP_SANDBOX_H

class Sandbox : public Hzn::Layer
{
public:
	Sandbox() : Layer("Sandbox") {}
	virtual ~Sandbox() = default;
	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate(Hzn::TimeStep deltaTime) override;
	virtual void onEvent(Hzn::Event& event) override;
	virtual void onRenderImgui() override;
private:

};

#endif