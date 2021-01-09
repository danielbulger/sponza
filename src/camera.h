#ifndef SPONZA_CAMERA_H
#define SPONZA_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sponza
{
	enum class CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera
	{
	public:

		Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

		void updateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane);

		[[nodiscard]]
		glm::mat4 getViewMatrix() const;

		[[nodiscard]]
		const glm::mat4 &getProjectionMatrix() const;

		void move(CameraMovement movement, float deltaTime);

		void rotate(float x, float y);

		void print() const;

		inline float getNearPlane() const
		{
			return m_nearPlane;
		}

		inline float getFarPlane() const
		{
			return m_farPlane;
		}

	private:

		glm::mat4 m_projectionMatrix{};

		glm::vec3 m_position;
		glm::vec3 m_front{};
		glm::vec3 m_up;
		glm::vec3 m_right{};
		glm::vec3 m_worldUp;

		float m_yaw;
		float m_pitch;

		float m_nearPlane;
		float m_farPlane;

		void updateCameraVectors();
	};
}

#endif
