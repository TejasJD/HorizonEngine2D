#pragma once

#ifndef HZN_COMPONENT_H
#define HZN_COMPONENT_H

#include <glm/glm.hpp>
#include "HorizonEngine/Camera/Camera.h"

namespace Hzn
{
	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const glm::mat4& glmTransform) : m_Transform(glmTransform) {}
		~TransformComponent() = default;

		// conversion operator for tranforms.
		operator const glm::mat4() const & { return m_Transform; }
		operator glm::mat4() & { return m_Transform; }
		glm::mat4 m_Transform = glm::mat4(1.0f);
	};

	struct RenderComponent
	{
		RenderComponent() = default;
		RenderComponent(const glm::vec4& color) : m_Color(color) {};
		~RenderComponent() = default;
		// conversion operator for render component.
		operator const glm::vec4() const & { return m_Color; }
		operator glm::vec4() & { return m_Color; }

		glm::vec4 m_Color = glm::vec4(1.0f);
	};

	struct CameraComponent
	{
		CameraComponent(const float aspectRatio, const float zoom)
			: m_Camera(aspectRatio, zoom) {}
		
		~CameraComponent() = default;
		SceneCamera2D m_Camera;
		bool m_Primary = true;
	};

}

#endif
