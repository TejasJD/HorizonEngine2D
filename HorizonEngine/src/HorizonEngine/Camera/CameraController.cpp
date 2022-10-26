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
		else if (Hzn::Input::keyPressed(Hzn::Key::Q))
		{
			m_Rotation -= (m_RotationSpeed * ts);
		}
		else if (Hzn::Input::keyPressed(Hzn::Key::E))
		{
			m_Rotation += (m_RotationSpeed * ts);
		}
		m_Camera.setRotation(m_Rotation);
		m_Camera.setPosition(m_Position);
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::onMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&OrthographicCameraController::onWindowResize, this, std::placeholders::_1));
	}


	bool OrthographicCameraController::onWindowResize(WindowResizeEvent& e)
	{
		m_Camera.setAspectRatio((float)e.GetWidth() / (float)e.GetHeight());
		return false;
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		HZN_CORE_INFO("(xOffset = {0}, yOffset = {1})", e.GetXOffset(), e.GetYOffset());
		float yOffset = e.GetYOffset();
		float zoom = m_Camera.getZoom();
		zoom -= (0.24f * yOffset);

		if (zoom < 1.0f) zoom = 1.0f;

		m_Camera.setZoom(zoom);
		HZN_CORE_INFO("{0}", m_Camera.getZoom());
		return false;
	}
}