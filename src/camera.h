#ifndef SPONZA_CAMERA_H
#define SPONZA_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sponza
{
	enum class CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera
	{
	public:

		Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

		[[nodiscard]]
		glm::mat4 getViewMatrix() const;

		void move(CameraMovement movement, float deltaTime);

		void rotate(float x, float y);

		void print();

	private:
		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		float m_yaw;
		float m_pitch;

		void updateCameraVectors();
	};
}

#endif
