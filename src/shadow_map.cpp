#include <shadow_map.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sponza
{
	const int NUM_FACES = 6;

	ShadowMap::ShadowMap(uint32_t width, uint32_t height) : m_width(width), m_height(height)
	{
		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

		for (int i = 0; i < NUM_FACES; ++i)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_DEPTH_COMPONENT,
				width,
				height,
				0,
				GL_DEPTH_COMPONENT,
				GL_FLOAT,
				nullptr
			);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	ShadowMap::~ShadowMap()
	{
		if (m_frameBuffer > 0)
		{
			glDeleteFramebuffers(1, &m_frameBuffer);
			m_frameBuffer = 0;
		}

		if (m_texture > 0)
		{
			glDeleteTextures(1, &m_texture);
			m_texture = 0;
		}
	}

	void ShadowMap::load(const Resource &resource)
	{
		m_shader = Shader(
			resource.getFilePath("/shadow.vert").c_str(),
			resource.getFilePath("/shadow.frag").c_str(),
			resource.getFilePath("/shadow.geom").c_str()
		);
	}

	void ShadowMap::bindForWrite(const PointLight &light, float farPlane) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glViewport(0, 0, m_width, m_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_shader.use();

		const glm::vec3 position = glm::vec3(light.position.x, light.position.y, light.position.z);

		glm::mat4 transforms[6] = {
			m_projection * glm::lookAt(position, position + glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0)),
			m_projection * glm::lookAt(position, position + glm::vec3(-1.0, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0)),
			m_projection * glm::lookAt(position, position + glm::vec3(0.0, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0)),
			m_projection * glm::lookAt(position, position + glm::vec3(0.0, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0)),
			m_projection * glm::lookAt(position, position + glm::vec3(0.0, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0)),
			m_projection * glm::lookAt(position, position + glm::vec3(0.0, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0))
		};

		m_shader.setVec4("lightPosition", light.position);
		m_shader.setFloat("farPlane", farPlane);
		m_shader.setMat4("matrix", glm::mat4(1.0f));

		for (int i = 0; i < 6; ++i)
		{
			m_shader.setMat4("shadowMatrices[" + std::to_string(i) + "]", transforms[i]);
		}
	}

	void ShadowMap::updateProjectionMatrix(float near, float far)
	{
		m_projection = glm::perspective(glm::radians(90.0f), (float) m_width / (float) m_height, near, far);
	}
}