#pragma once

#ifndef HZN_INPUT_H
#define HZN_INPUT_H

namespace Hzn
{
	class Input
	{
	public:
		virtual ~Input() {}
		static Input* create();
		inline static bool keyPressed(int key) { return m_Instance->keyPressedUtil(key); }
		inline static bool mouseButtonPresssed(int button) { return m_Instance->mouseButtonPressedUtil(button); }
		inline static std::pair<double, double> getMousePos() { return m_Instance->getMousePosUtil(); }
		inline static double getMouseX() { return m_Instance->getMouseXUtil(); }
		inline static double getMouseY() { return m_Instance->getMouseYUtil(); }

	protected:
		virtual bool keyPressedUtil(int key) const = 0;
		virtual bool mouseButtonPressedUtil(int button) const = 0;
		virtual std::pair<double, double> getMousePosUtil() const = 0;
		virtual double getMouseXUtil() const = 0;
		virtual double getMouseYUtil() const = 0;

	private:
		static Input* m_Instance;
	};
}

#endif