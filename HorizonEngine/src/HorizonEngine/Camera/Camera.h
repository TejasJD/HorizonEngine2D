#pragma once

#ifndef HZN_CAMERA_H
#define HZN_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <cereal/access.hpp>

namespace Hzn
{

	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;

		virtual glm::mat4& getProjectionMatrix() = 0;
		virtual const glm::mat4& getProjectionMatrix() const = 0;
		virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) = 0;
	};

	/// <summary>
	/// Represents an Orthographic Camera, (Usually used as a 2D camera).
	/// </summary>
	class OrthographicCamera : public Camera
	{
	public:
		/// <summary>
		/// Creates an Orthographic camera.
		/// </summary>
		/// <param name="aspectRatio">: Provide the aspect ratio of the screen, which can be calculated as width / height of the screen.</param>
		/// <param name="zoom">: Default value is 1.0f.</param>
		/// <param name="position">: Defaults value is (0, 0, 0). Position of the Camera in the world.</param>
		OrthographicCamera(
			float aspectRatio,
			float zoom = 1.0f,
			const glm::vec3& position = { 0.0f, 0.0f, 0.0f }
		) : m_AspectRatio(aspectRatio), m_Zoom(zoom), m_Position(position)
		{
			calculateViewMatrix();
			calculateProjectionMatrix();
		}

		virtual ~OrthographicCamera() override = default;
		/// <summary>
		/// Current view matrix of the camera (it is the inverse of the Camera's transform / model matrix).
		/// Is recalculated when the position or the orientation of the camera changes.
		/// </summary>
		/// <returns>Reference to the view matrix (4 x 4).</returns>
		glm::mat4& getViewMatrix() { return m_ViewMatrix; }
		/// <summary>
		/// Current projection matrix of the camera.
		/// Is recalculated when the Aspect Ratio or the Zoom of the camera changes.
		/// </summary>
		/// <returns>Reference to the projection matrix of the camera (4 x 4).</returns>
		virtual glm::mat4& getProjectionMatrix() override { return m_ProjectionMatrix; }
		/// <summary>
		/// Current view matrix of the camera (it is the inverse of the Camera's transform / model matrix).
		/// Is recalculated when the position or the orientation of the camera changes.
		/// (This function is invoked when called on constant camera object).
		/// </summary>
		/// <returns>Constant reference to the view matrix (4 x 4).</returns>
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		/// <summary>
		/// Current projection matrix of the camera.
		/// Is recalculated when the Aspect Ratio or the Zoom of the camera changes.
		/// (This function is invoked when called on constant object).
		/// </summary>
		/// <returns>Const Reference to the projection matrix of the camera (4 x 4).</returns>
		const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
		/// <summary>
		/// Utility function to calculate the view matrix of the camera.
		/// Recalculation of the view matrix is required when the position or the orientation of the camera changes.
		/// </summary>
		void calculateViewMatrix()
		{
			//HZN_CORE_WARN("Calculating view matrix...");
			const glm::mat4 cameraTransform = 
				glm::translate(glm::mat4(1.0f), m_Position) * 
				glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			m_ViewMatrix = glm::inverse(cameraTransform);
		}
		/// <summary>
		/// Utility function to calculate the projection matrix of the camera.
		/// Recalculation of the projection matrix is required when the aspect ratio of the screen or the the
		/// Zoom of the camera changes.
		/// </summary>
		void calculateProjectionMatrix()
		{
			//HZN_CORE_INFO("Calculating projection matrix...");
			m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom
				, -m_Zoom, m_Zoom, -1.0f, 1.0f);
		}

		virtual void setProjectionMatrix(const glm::mat4& mat) override
		{
			m_ProjectionMatrix = mat;
		}

		/// <summary>
		/// Get the aspect ratio value that the camera currently holds.
		/// </summary>
		/// <returns>The aspect ratio known by the camera.</returns>
		float getAspectRatio() const { return m_AspectRatio; }
		/// <summary>
		/// Get the zoom value of the camera. For an orthographic camera, it is the zoom value of the camera.
		/// Would Usually range from [+1.0f, +Inf).
		/// </summary>
		/// <returns>Field of View of the camera.</returns>
		float getZoom() const { return m_Zoom; }
		/// <summary>
		/// Set the known aspect ratio to a new value when the aspect ratio of the screen changes.
		/// Recalculates the projection matrix of the camera.
		/// </summary>
		/// <param name="aspectRatio">: The new aspect ratio of the camera.</param>
		void setAspectRatio(float aspectRatio)
		{
			if (std::fabs(m_AspectRatio - aspectRatio) > 1e-6f)
			{
				m_AspectRatio = aspectRatio;
				calculateProjectionMatrix();
			}
		}
		/// <summary>
		/// Set the known zoom value to a new a value when the zoom value of the orthographic camera changes.
		/// Recalculates the projection matrix of the camera.
		/// </summary>
		/// <param name="zoom">: The new zoom value of the camera</param>
		void setZoom(float zoom)
		{
			if (std::fabs(m_Zoom - zoom) > 1e-6f)
			{
				m_Zoom = zoom;
				calculateProjectionMatrix();
			}
		}
		/// <summary>
		/// Get the position of the camera.
		/// </summary>
		/// <returns>Position vector of the camera (a 3D vector).</returns>.
		glm::vec3 getPosition() const { return m_Position; }
		/// <summary>
		/// Set the position of the camera to the given position.
		/// Recalculates the projection matrix.
		/// </summary>
		/// <param name="position">: 3D Position vector of the camera.</param>
		void setPosition(const glm::vec3& position)
		{
			if (m_Position != position)
			{
				m_Position = position;
				calculateViewMatrix();
			}
		}

		float getRotation() const { return m_Rotation; }

		void setRotation(float angle)
		{
			if (std::fabs(m_Rotation - angle) > 1e-6f)
			{
				m_Rotation = angle;
				calculateViewMatrix();
			}
		}

	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_AspectRatio = 1.0f;
		float m_Zoom = 1.0f;
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};

	/// <summary>
	/// Represents a Perspective Camera (Usually used as a 3D camera).
	/// </summary>
	class PerspectiveCamera : public Camera
	{
	public:
		/// <summary>
		/// Create a Perspective Camera.
		/// </summary>
		/// <param name="aspectRatio">: Provide the aspect ratio of the screen, which can be calculated as width / height of the screen.</param>
		/// <param name="fov">: Default value is 45 degrees. Field of View of the Camera (in degrees), increasing the field of view zooms out and decreasing zooms in. Usually ranges from
		/// 45 degrees to 65 degrees</param>
		/// <param name="nearPlane">: Default value is 0.1f, The near plane of the perspective projection, 
		/// a 2D plane on which objects in the 3D world are projected.</param>
		/// <param name="farPlane">: Default value is 100.0f, The far plane of the perspective projection, anything object this 
		/// plane is not visible in the given frame.</param>
		/// <param name="position">: Defaults value is (0, 0, 0). Position of the Camera in the world.</param>
		PerspectiveCamera(
			float aspectRatio,
			float fov = 45.0f,
			float nearPlane = 0.1f,
			float farPlane = 100.0f,
			const glm::vec3& position = { 0.0f, 0.0f, 0.0f }
		) : m_AspectRatio(aspectRatio), m_Fov(fov), m_Near(nearPlane), m_Far(farPlane), m_Position(position)
		{
			calculateViewMatrix();
			calculateProjectionMatrix();
		}

		virtual ~PerspectiveCamera() override = default;
		/// <summary>
		/// Current view matrix of the camera (it is the inverse of the Camera's transform / model matrix).
		/// Is recalculated when the position or the orientation of the camera changes.
		/// </summary>
		/// <returns>Reference to the view matrix (4 x 4).</returns>
		glm::mat4& getViewMatrix(){ return m_ViewMatrix; }
		/// <summary>
		/// Current projection matrix of the camera.
		/// Is recalculated when the Aspect Ratio or the Field of View of the camera changes.
		/// </summary>
		/// <returns>Reference to the projection matrix of the camera (4 x 4).</returns>
		virtual glm::mat4& getProjectionMatrix() override { return m_ProjectionMatrix; }
		/// <summary>
		/// Current view matrix of the camera (it is the inverse of the Camera's transform / model matrix).
		/// Is recalculated when the position or the orientation of the camera changes.
		/// (This function is invoked when called on constant camera object).
		/// </summary>
		/// <returns>Constant reference to the view matrix (4 x 4).</returns>
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		/// <summary>
		/// Current projection matrix of the camera.
		/// Is recalculated when the Aspect Ratio or the Field of View of the camera changes.
		/// (This function is invoked when called on constant object).
		/// </summary>
		/// <returns>Const Reference to the projection matrix of the camera (4 x 4).</returns>
		virtual const glm::mat4& getProjectionMatrix() const override { return m_ProjectionMatrix; }

		/// <summary>
		/// Utility function to calculate the view matrix of the camera.
		/// Recalculation of the view matrix is required when the position or the orientation of the camera changes.
		/// </summary>
		void calculateViewMatrix()
		{
			glm::mat4 cameraTransform = glm::mat4(1.0f);
			cameraTransform = glm::translate(cameraTransform, m_Position);
			m_ViewMatrix = glm::inverse(cameraTransform);
		}
		/// <summary>
		/// Utility function to calculate the projection matrix of the camera.
		/// Recalculation of the projection matrix is required when the aspect ratio of the screen or the the
		/// Field of view of the camera changes.
		/// </summary>
		void calculateProjectionMatrix()
		{
			m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
		}

		virtual void setProjectionMatrix(const glm::mat4& mat) override
		{
			m_ProjectionMatrix = mat;
		}

		/// <summary>
		/// Get the aspect ratio value that the camera currently holds.
		/// </summary>
		/// <returns>The aspect ratio known by the camera.</returns>
		float getAspectRatio() const { return m_AspectRatio; }
		/// <summary>
		/// Get the zoom value of the camera. For a perspective camera, it is field of view of the camera in degrees.
		/// </summary>
		/// <returns>Field of View of the camera.</returns>
		float getZoom() const { return m_Fov; }
		/// <summary>
		/// Set the known aspect ratio of the camera when the aspect ratio of the screen changes.
		/// This recalculates the projection matrix of the camera.
		/// </summary>
		/// <param name="aspectRatio"></param>
		void setAspectRatio(float aspectRatio)
		{
			if (std::fabs(m_AspectRatio - aspectRatio) > 1e-6)
			{
				m_AspectRatio = aspectRatio;
				calculateProjectionMatrix();
			}
		}
		/// <summary>
		/// Set the known Fiel of View value to a new value when the Field of View of the perspective camera changes.
		/// This recalculates the projection matrix of the camera.
		/// </summary>
		/// <param name="zoom"></param>
		void setZoom(float fov)
		{
			if (std::fabs(m_Fov - fov) > 1e-6)
			{
				m_Fov = fov;
				calculateProjectionMatrix();
			}
		}
		/// <summary>
		/// Get the position of the camera.
		/// </summary>
		/// <returns>Position vector of the camera (a 3D vector).</returns>
		glm::vec3 getPosition() const { return m_Position; }
		/// <summary>
		/// Set the position of the camera to the given position.
		/// Recaculates the projection matrix.
		/// </summary>
		/// <param name="position">: 3D Position vector of the camera.</param>
		void setPosition(const glm::vec3& position)
		{
			if (position != m_Position)
			{
				m_Position = position;
				calculateViewMatrix();
			}
		}

		float getRotation() const { return m_Rotation; }

		void setRotation(float angle)
		{
			if (std::fabs(m_Rotation - angle) > 1e-6f)
			{
				m_Rotation = angle;
				calculateViewMatrix();
			}
		}

	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		float m_Rotation = 0.0f;
		float m_AspectRatio = 1.0f;
		float m_Fov = 1.0f;
		float m_Near = 0.1f;
		float m_Far = 100.0f;
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};

	class SceneCamera2D : public Camera
	{
	public:
		/*friend class cereal::access;*/
		SceneCamera2D(const float aspectRatio, const float zoom) :
			m_AspectRatio(aspectRatio),
			m_Zoom(zoom)
		{
			calculateProjectionMatrix();
		}

		virtual ~SceneCamera2D() override = default;

		virtual glm::mat4& getProjectionMatrix() override { return m_Projection; }
		virtual const glm::mat4& getProjectionMatrix() const override { return m_Projection; }
		virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) override { m_Projection = projectionMatrix; }

		void calculateProjectionMatrix()
		{
			m_Projection = glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom, -1.0f, 1.0f);
		}

		float getAspectRatio() const { return m_AspectRatio; }
		float getZoom() const { return m_Zoom; }
		void setAspectRatio(const float aspectRatio)
		{
			m_AspectRatio = aspectRatio;
			calculateProjectionMatrix();
		}
		void setZoom(const float zoom)
		{
			m_Zoom = zoom;
			calculateProjectionMatrix();
		}

		template<typename Archive>
		void load(Archive& ar)
		{
			ar(m_Zoom);

			for(int i = 0; i < 4; ++i)
			{
				ar(m_Projection[i].x, m_Projection[i].y, m_Projection[i].z, m_Projection[i].w);
			}
		}

		template<typename Archive>
		void save(Archive& ar) const
		{
			ar(m_Zoom);

			for (int i = 0; i < 4; ++i)
			{
				ar(m_Projection[i].x, m_Projection[i].y, m_Projection[i].z, m_Projection[i].w);
			}
		}

	private:
		float m_AspectRatio = 1.0f;
		float m_Zoom = 1.0f;
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};

}

#endif