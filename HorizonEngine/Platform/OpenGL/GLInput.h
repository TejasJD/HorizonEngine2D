#pragma once

#ifndef HZN_MS_INPUT_H
#define HZN_MS_INPUT_H

#include "HorizonEngine/Input.h"

namespace Hzn
{
	class GLInput : public Input
	{
	protected:
		virtual bool keyPressedUtil(int key) const override;
		virtual bool mouseButtonPressedUtil(int button) const override;
		virtual std::pair<double, double> getMousePosUtil() const override;
		virtual double getMouseXUtil() const override;
		virtual double getMouseYUtil() const override;

	private:

	};
}

#endif