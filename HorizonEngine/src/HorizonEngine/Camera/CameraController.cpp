#include "pch.h"
#include "CameraController.h"

namespace Hzn
{
	void OrthographicCameraController::onUpdate(TimeStep ts)
	{
		/*if (Hzn::Input::keyPressed(Hzn::Key::W))
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
		}*/

		if(m_MousePressed)
		{
			glm::vec2 currentMousePos{ Input::getMouseX(), Input::getMouseY() };
			glm::vec2 mouseDelta = { lastMousePos.x - currentMousePos.x, currentMousePos.y - lastMousePos.y };
			lastMousePos = currentMousePos;
			m_Position += m_TranslationSpeed * 0.3f * ts * glm::vec3{ mouseDelta.x, mouseDelta.y, 0.0f };
		}

		m_Camera.setRotation(m_Rotation);
		m_Camera.setPosition(m_Position);
		m_TranslationSpeed = m_Camera.getZoom();
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::onMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&OrthographicCameraController::onWindowResize, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&OrthographicCameraController::onMouseButtonPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&OrthographicCameraController::onMouseButtonReleased, this, std::placeholders::_1));
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
		zoom -= (0.25f * yOffset);

		if (zoom < 0.25f) zoom = 0.25f;

		m_Camera.setZoom(zoom);
		//HZN_CORE_INFO("{0}", m_Camera.getZoom());
		return false;
	}

	bool OrthographicCameraController::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		/*HZN_CORE_INFO("{0}", e.GetMouseButton());*/
		auto mouseButton = e.GetMouseButton();
		if(mouseButton == Mouse::ButtonRight)
		{
			lastMousePos = { Input::getMouseX(), Input::getMouseY() };
			m_MousePressed = true;
		}
		return false;
	}

	bool OrthographicCameraController::onMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		m_MousePressed = false;
		return false;
	}
}
