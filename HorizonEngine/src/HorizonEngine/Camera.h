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
			const glm::vec3& position = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& front = { 0.0f, 0.0f, -1.0f },
			const glm::vec3& up = { 0.0f, 1.0f, 0.0f }
		);
		~Camera();

		glm::vec3 getPosition() const { return m_Position; }
		glm::vec3 getFront() const { return m_Front; }
		glm::vec3 getUp() const { return m_Up; }
		glm::vec3 getRight() const { return glm::normalize(glm::cross(m_Front, m_Up)); }
		
		float getSpeed() const { return m_Speed; }
		float getDeltaTime() const { return m_DeltaTime; }
		float getFov() const { return m_Fov; }
		float getAspectRatio() const { return m_AspectRatio; }

		void setPosition(const glm::vec3& position) { m_Position = position; }
		void setFront(const glm::vec3& front) 
		{ 
			m_Front = front;
			glm::vec3 right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
			setUp(glm::normalize(glm::cross(right, m_Front)));
		}
		void setUp(const glm::vec3& up) { m_Up = up; }
		void setFov(const float& fov) { m_Fov = fov; }
		void setAspectRatio(const float& aspectRatio) { m_AspectRatio = aspectRatio; }

		void setDeltaTime(const float& deltaTime) { m_DeltaTime = deltaTime; }
		void setSpeed(const float& speed) { m_Speed = speed; }

		void moveFront()
		{
			m_Position += (m_Speed * m_DeltaTime) * m_Front;
		}

		void moveBack() 
		{
			m_Position -= (m_Speed * m_DeltaTime) * m_Front;
		}

		void moveRight()
		{
			m_Position += (m_Speed * m_DeltaTime) * getRight();
		}

		void moveLeft()
		{
			m_Position -= (m_Speed * m_DeltaTime) * getRight();
		}

		glm::mat4 getViewMatrix() const 
		{
			return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		}

		glm::mat4 getPerspectiveProjectionMatrix(const float& nearPlane, const float& farPlane) const
		{
			return glm::perspective(glm::radians(m_Fov), m_AspectRatio, nearPlane, farPlane);
		}

		glm::mat4 getOrthographicProjectionMatrix(const float& left, const float& right, const float&
			bottom, const float& top)
		{
			return glm::ortho(left, right, bottom, top, 0.1f, 100.0f);
		}

	private:
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		glm::vec3 m_Front = {0.0f, 0.0f, -1.0f};
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		float m_Speed = 0.0f;
		float m_DeltaTime = 0.0f;
		float m_Fov = 0.0f;
		float m_AspectRatio = 0.0f;
	};
}

#endif