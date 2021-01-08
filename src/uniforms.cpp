#include <uniforms.h>
#include <glad/glad.h>

namespace sponza
{

	Uniforms::Uniforms(int shaderId, int normalMapShaderId, const Camera &camera, sponza::PointLight *lights)
	{
		glUniformBlockBinding(shaderId, glGetUniformBlockIndex(shaderId, "CameraBlock"), 0);
		glUniformBlockBinding(shaderId, glGetUniformBlockIndex(shaderId, "LightBlock"), 1);

		glUniformBlockBinding(normalMapShaderId, glGetUniformBlockIndex(normalMapShaderId, "CameraBlock"), 0);
		glUniformBlockBinding(normalMapShaderId, glGetUniformBlockIndex(normalMapShaderId, "LightBlock"), 1);

		m_uniforms.projectionMatrix = camera.getProjectionMatrix();
		m_uniforms.viewMatrix = camera.getViewMatrix();
		m_uniforms.normalMatrix = glm::transpose(glm::inverse(m_uniforms.viewMatrix));

		glGenBuffers(2, m_uniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(GlobalUniforms), &m_uniforms, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uniformBuffer[0], 0, sizeof(GlobalUniforms));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer[1]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(sponza::PointLight) * 4, lights, GL_STATIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_uniformBuffer[1], 0, sizeof(sponza::PointLight) * 4);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	Uniforms::~Uniforms()
	{
		glDeleteBuffers(2, m_uniformBuffer);
	}

	void Uniforms::update(const Camera &camera)
	{
		m_uniforms.projectionMatrix = camera.getProjectionMatrix();
		m_uniforms.viewMatrix = camera.getViewMatrix();
		m_uniforms.normalMatrix = glm::transpose(glm::inverse(m_uniforms.viewMatrix));

		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(GlobalUniforms), &m_uniforms, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}