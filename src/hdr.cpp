#include <hdr.h>
#include <glad/glad.h>
#include "mesh.h"

namespace sponza
{

	HDR::HDR(int width, int height) : m_frameBuffer(0), m_colourBuffer(0), m_renderBuffer(0), m_quadVao(0)
	{
		glGenFramebuffers(1, &m_frameBuffer);

		glGenTextures(1, &m_colourBuffer);
		glBindTexture(GL_TEXTURE_2D, m_colourBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_quadVao = sponza::InitQuadMesh();
	}

	HDR::~HDR()
	{
		if (m_frameBuffer > 0)
		{
			glDeleteFramebuffers(1, &m_frameBuffer);
			m_frameBuffer = 0;
		}

		if (m_renderBuffer > 0)
		{
			glDeleteRenderbuffers(1, &m_renderBuffer);
			m_renderBuffer = 0;
		}

		if (m_colourBuffer > 0)
		{
			glDeleteTextures(1, &m_colourBuffer);
			m_colourBuffer = 0;
		}

		if (m_quadVao > 0)
		{
			glDeleteVertexArrays(1, &m_quadVao);
			m_quadVao = 0;
		}
	}

	void HDR::bindForWrite()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void HDR::render()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_shader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colourBuffer);
		glBindVertexArray(m_quadVao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void HDR::load(const Resource &resource)
	{
		m_shader = Shader(
			resource.getFilePath("/hdr.vert").c_str(),
			resource.getFilePath("/hdr.frag").c_str()
		);
	}
}