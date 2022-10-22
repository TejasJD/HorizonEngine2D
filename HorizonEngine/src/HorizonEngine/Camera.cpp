#include "pch.h"
#include "Camera.h"


namespace Hzn
{
	Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
		: m_Position(position), m_Front(front), m_Up(up)
	{
		
	}

	Camera::~Camera()
	{

	}
	
}