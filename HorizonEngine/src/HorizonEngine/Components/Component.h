#pragma once

#ifndef HZN_COMPONENT_H
#define HZN_COMPONENT_H

#include <glm/glm.hpp>
#include "HorizonEngine/Camera/Camera.h"

namespace Hzn
{
	struct NameComponent
	{
		NameComponent() = default;
		NameComponent(const NameComponent& name) = default;
		NameComponent(const std::string& name): m_Name(name) {}
		~NameComponent() = default;

		operator std::string() const & { return m_Name; }
		operator std::string()& { return m_Name; }
		std::string m_Name;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_Name);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_Name);
		}
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const glm::mat4& glmTransform) : m_Transform(glmTransform) {}
		~TransformComponent() = default;

		// conversion operator for tranforms.
		operator const glm::mat4() const & { return m_Transform; }
		operator glm::mat4() & { return m_Transform; }
		glm::mat4 m_Transform = glm::mat4(1.0f);

		template<typename Archive>
		void load(Archive& ar)
		{
			for(int i = 0; i < m_Transform.length(); ++i)
			{
				ar(m_Transform[i].x, m_Transform[i].y, m_Transform[i].z, m_Transform[i].w);
			}
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			for (int i = 0; i < m_Transform.length(); ++i)
			{
				ar(m_Transform[i].x, m_Transform[i].y, m_Transform[i].z, m_Transform[i].w);
			}
		}
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

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		}
	};

	struct CameraComponent
	{
		CameraComponent(const float aspectRatio = 1.0f, const float zoom = 1.0f)
			: m_Camera(aspectRatio, zoom) {}
		
		~CameraComponent() = default;
		SceneCamera2D m_Camera;
		bool m_Primary = true;

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_Camera, m_Primary);
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_Camera, m_Primary);
		}
	};

}

#endif
