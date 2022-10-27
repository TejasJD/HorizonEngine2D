#include "pch.h"
#include "CameraController.h"

namespace Hzn
{
	void OrthographicCameraController::onUpdate(TimeStep ts)
	{
		if (Hzn::Input::keyPressed(Hzn::Key::W))
		{
			m_Position.y += (m_TranslationSpeed * ts);
		}
		else if (Hzn::Input::keyPressed(Hzn::Key::A))
		{
			m_Position.x -= (m_TranslationSpeed * ts);
		}
		else if (Hzn::Input::keyPressed(Hzn::Key::S))
		{
			m_Position.y -= (m_TranslationSpeed * ts);
		}
		else if (Hzn::Input::keyPressed(Hzn::Key::D))
		{
			m_Position.x += (m_TranslationSpeed * ts);
		}


		if (m_CanRotate)
		{
			if (Hzn::Input::keyPressed(Hzn::Key::Q))
			{
				m_Rotation -= (m_RotationSpeed * ts);
			}
			else if (Hzn::Input::keyPressed(Hzn::Key::E))
			{
				m_Rotation += (m_RotationSpeed * ts);
			}
		}

		if (m_MouseDrag)
		{
			if (Hzn::Input::mouseButtonPresssed(Hzn::Mouse::ButtonLeft))
			{
				auto [x, y] = Hzn::Input::getMousePos();
				if (!mousePressed)
				{
					lastX = x;
					lastY = y;
					mousePressed = true;
				}

				float xOffset = lastX - (float)x;
				float yOffset = lastY - (float)y;
				lastX = x;
				lastY = y;
				HZN_CORE_WARN("({0}, {1})", x, y);
				HZN_CORE_INFO("({0}, {1})", xOffset, yOffset);
				m_Position.x += (xOffset * 0.4f * ts);
				m_Position.y -= (yOffset * 0.4f * ts);
			}
			else {
				mousePressed = false;
			}
		}

		m_Camera.setRotation(m_Rotation);
		m_Camera.setPosition(m_Position);
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::onMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&OrthographicCameraController::onWindowResize, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&OrthographicCameraController::onMouseButtonPressed, this, std::placeholders::_1));
	}


	bool OrthographicCameraController::onWindowResize(WindowResizeEvent& e)
	{
		m_Camera.setAspectRatio((float)e.GetWidth() / (float)e.GetHeight());
		return false;
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		//HZN_CORE_INFO("(xOffset = {0}, yOffset = {1})", e.GetXOffset(), e.GetYOffset());
		float yOffset = e.GetYOffset();
		float zoom = m_Camera.getZoom();
		zoom -= (0.24f * yOffset);

		if (zoom < 1.0f) zoom = 1.0f;

		m_Camera.setZoom(zoom);
		//HZN_CORE_INFO("{0}", m_Camera.getZoom());
		return false;
	}

	bool OrthographicCameraController::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		/*HZN_CORE_INFO("{0}", e.GetMouseButton());*/
		return false;
	}
}