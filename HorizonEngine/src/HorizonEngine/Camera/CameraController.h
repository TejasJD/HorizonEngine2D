#pragma once

#ifndef HZN_CAMERA_CONTROLLER_H
#define HZN_CAMERA_CONTROLLER_H

#include "HorizonEngine/Core/TimeStep.h"
#include "HorizonEngine/Events/Event.h"
#include "HorizonEngine/Events/ApplicationEvent.h"
#include "HorizonEngine/Events/MouseEvent.h"
#include "HorizonEngine/Events/KeyEvent.h"
#include "HorizonEngine/Input.h"

#include "Camera.h"

namespace Hzn
{
	//! Provides a controller for camera, just to implement camera.
	class CameraController
	{
	public:
		CameraController() = default;
		virtual ~CameraController() = default;
		//! keep track of changes made to the Camera on every frame.
		virtual void onUpdate(TimeStep ts) = 0;
		//! function to dispatch events that come the Camera.
		virtual void onEvent(Event& e) = 0;
		//! returns a reference to the camera bound to the controller.
		virtual Camera& getCamera() = 0;
		//! returns a constant reference to the camera bound to the controller (When the controller is a constant object).
		virtual const Camera& getCamera() const = 0;

		virtual void setTranslationSpeed(float translationSpeed) = 0;
		virtual void setRotationSpeed(float rotationSpeed) = 0;

		virtual float getTranslationSpeed() const = 0;
		virtual float getRotationSpeed() const = 0;
		virtual void enableRotation(bool flag) = 0;
		virtual void enableMouseDragMovement(bool flag) = 0;

	private:
		virtual bool onWindowResize(WindowResizeEvent& e) = 0;
		virtual bool onMouseScrolled(MouseScrolledEvent& e) = 0;
		virtual bool onMouseButtonPressed(MouseButtonPressedEvent& e) = 0;
	};
	
	//! Orthographic Camera Controller. Usually used in 2D scenarios. Wrapper around an Orthographic Camera.
	//! Allows user to control camera.
	class OrthographicCameraController : public CameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, float zoom)
			: m_Camera(OrthographicCamera(aspectRatio, zoom))
		{}
		virtual ~OrthographicCameraController() = default;
		// Inherited via CameraController

		//! keep track of changes made to the Camera on every frame.
		virtual void onUpdate(TimeStep ts) override;
		//! process events received by the Camera.
		virtual void onEvent(Event& e) override;
		//! returns a reference to the camera object.
		virtual Camera& getCamera() override { return m_Camera; }
		//! returns a reference to the constant camera object (when the member function is called on constant camera object.
		virtual const Camera& getCamera() const override { return m_Camera; }

		virtual void setTranslationSpeed(float translationSpeed) override
		{
			m_TranslationSpeed = translationSpeed;
		}
		virtual void setRotationSpeed(float rotationSpeed) override
		{
			m_RotationSpeed = rotationSpeed;
		}

		virtual float getTranslationSpeed() const override { return m_TranslationSpeed; }
		virtual float getRotationSpeed() const override { return m_RotationSpeed; }

		virtual void enableRotation(bool flag) override { m_CanRotate = flag; }

		virtual void enableMouseDragMovement(bool flag) override { m_MouseDrag = flag; }

	private:
		virtual bool onWindowResize(WindowResizeEvent& e) override;
		virtual bool onMouseScrolled(MouseScrolledEvent& e) override;
		virtual bool onMouseButtonPressed(MouseButtonPressedEvent& e) override;

		OrthographicCamera m_Camera;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_TranslationSpeed = 2.5f;
		float m_RotationSpeed = 4.0f;
		float lastX = 0.0f;
		float lastY = 0.0f;

		bool mousePressed = false;
		bool m_MouseDrag = false;
		bool m_CanRotate = false;
	};
}

#endif // !HZN_CAMERA_CONTROLLER_H
