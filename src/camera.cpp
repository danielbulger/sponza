#include <camera.h>
#include <iostream>

namespace sponza
{

	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
		m_position(position), m_up(up), m_worldUp(up), m_yaw(yaw), m_pitch(pitch)
	{
		updateCameraVectors();
	}

	void Camera::updateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane)
	{
		m_farPlane = farPlane;
		m_nearPlane = nearPlane;

		m_projectionMatrix = glm::perspective(
			glm::radians(fov),
			aspect,
			nearPlane,
			farPlane
		);
	}

	glm::mat4 Camera::getViewMatrix() const
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	void Camera::updateCameraVectors()
	{
		glm::vec3 front;

		front.x = cos(static_cast<double>(glm::radians(m_yaw))) * cos(static_cast<double>(glm::radians(m_pitch)));
		front.y = sin(static_cast<double>(glm::radians(m_pitch)));
		front.z = sin(static_cast<double>(glm::radians(m_yaw))) * cos(static_cast<double>(glm::radians(m_pitch)));

		m_front = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}

	void Camera::move(CameraMovement movement, float deltaTime)
	{
		const float velocity = 1000.0f * deltaTime;

		switch (movement)
		{
			case CameraMovement::FORWARD:
				m_position += m_front * velocity;
				break;
			case CameraMovement::BACKWARD:
				m_position -= m_front * velocity;
				break;
			case CameraMovement::LEFT:
				m_position -= m_right * velocity;
				break;
			case CameraMovement::RIGHT:
				m_position += m_right * velocity;
				break;
		}

		updateCameraVectors();
	}

	void Camera::rotate(float x, float y)
	{
		m_yaw += (x * 0.01f);
		m_pitch += (y * 0.01f);

		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		else if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}

		updateCameraVectors();
	}

	void Camera::print() const
	{
		std::cout << m_position[0] << ',' << m_position[1] << ',' << m_position[2] << std::endl;
	}

	const glm::mat4& Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	glm::vec4 NormalisePlane(const glm::vec4 &plane)
	{
		const float l = glm::length(glm::vec3(plane.x, plane.y, plane.z));
		return plane / l;
	}

	void Camera::extractFrustum(glm::vec4 planes[6])
	{
		const glm::mat4 matrix = glm::transpose(m_projectionMatrix * getViewMatrix());

		float sign = 1.0;
		for(int i = 0; i < 6; ++i)
		{
			const int idx = (i & ~1) >> 1;
			planes[i].x = matrix[3][0] + (sign * matrix[idx][0]);
			planes[i].y = matrix[3][1] + (sign * matrix[idx][1]);
			planes[i].z = matrix[3][2] + (sign * matrix[idx][2]);
			planes[i].w = matrix[3][3] + (sign * matrix[idx][3]);
			planes[i] = NormalisePlane(planes[i]);
			sign *= -1.0f;
		}
	}
}