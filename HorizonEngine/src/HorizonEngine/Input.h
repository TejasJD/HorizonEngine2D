
#pragma once

#ifndef HZN_INPUT_H
#define HZN_INPUT_H

namespace Hzn
{
	enum class CursorMode : uint8_t
	{
		Enabled = 0,
		Disabled = 1,
		Hidden = 2
	};

	class Input
	{
	public:
		virtual ~Input() {}
		static std::shared_ptr<Input> create();
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
		static std::shared_ptr<Input> m_Instance;
	};
}

#endif