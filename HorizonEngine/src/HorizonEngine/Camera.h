#pragma once

#ifndef HZN_CAMERA_H
#define HZN_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hzn
{

	class Camera
	{
	public:
		Camera(
			const glm::vec3& _position,
			const glm::vec3& _front,
			const glm::vec3& _up
		): position(_position), front(_front), up(_up) {}

		virtual glm::mat4 getViewMatrix() const = 0;
		virtual glm::mat4 getProjectionMatrix() const = 0;
		virtual ~Camera() = default;

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };

		float speed = 2.5f;
		float deltaTime = 1.0f / 60.0f; // defaults to 60hz for now;

	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(
			float left,
			float right,
			float bottom,
			float top,
			float nearPlane = 0.0f,
			float farPlane = 100.0f,
			const glm::vec3& position = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& front = { 0.0f, 0.0f, -1.0f },
			const glm::vec3& up = { 0.0f, 1.0f, 0.0f }
		): m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top), m_Near(nearPlane), m_Far(farPlane), Camera(position, front, up) {}

		void setCameraParameters(float left, float right, float bottom, float top, float nearPlane = 0.0f, float farPlane = 100.0f)
		{
			m_Left = left;
			m_Right = right;
			m_Bottom = bottom;
			m_Top = top;
			m_Near = nearPlane;
			m_Far = farPlane;
		}

		virtual glm::mat4 getViewMatrix() const override
		{
			return glm::lookAt(position, position + front, up);
		}

		virtual glm::mat4 getProjectionMatrix() const override
		{
			return glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
		}

	private:
		float m_Left = -1.0f;
		float m_Right = 1.0f;
		float m_Bottom = -1.0f;
		float m_Top = 1.0f;
		float m_Near;
		float m_Far;

	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(
			float fov,
			float aspectRatio,
			float nearPlane,
			float farPlane,
			const glm::vec3& position = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& front = { 0.0f, 0.0f, -1.0f },
			const glm::vec3& up = { 0.0f, 1.0f, 0.0f }
		) : m_Fov(fov), m_AspectRatio(aspectRatio), m_Near(nearPlane), m_Far(farPlane), Camera(position, front, up) {}

		virtual ~PerspectiveCamera() = default;

		virtual glm::mat4 getViewMatrix() const override
		{
			return glm::lookAt(position, position + front, up);
		}

		virtual glm::mat4 getProjectionMatrix() const override
		{
			return glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
		}

	private:
		float m_Fov;
		float m_AspectRatio;
		float m_Near;
		float m_Far;
	};
}

#endif