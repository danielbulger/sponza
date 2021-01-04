#include <skybox.h>
#include "loader.h"

namespace sponza
{
	const float VERTICES[] = {

		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	Skybox::~Skybox()
	{
		glDeleteTextures(1, &m_texture);
		glDeleteBuffers(1, &m_buffer);
		glDeleteVertexArrays(1, &m_vao);
	}

	void Skybox::render(const Camera &camera, const glm::mat4 &projection)
	{
		glDepthFunc(GL_LEQUAL);
		m_shader.use();

		m_shader.setMat4("projection", projection);
		m_shader.setMat4("view", glm::mat4(glm::mat3(camera.getViewMatrix())));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		m_shader.setInt("skybox", 0);

		glBindVertexArray(m_vao);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(VERTICES) / 3);

		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

	void Skybox::load()
	{
		m_shader = Shader(
			"/home/daniel/Workspace/Sponza/data/skybox.vert",
			"/home/daniel/Workspace/Sponza/data/skybox.frag"
		);

		const std::vector<std::string> faces = {
			"/home/daniel/Workspace/Sponza/data/skybox/right.jpg",
			"/home/daniel/Workspace/Sponza/data/skybox/left.jpg",

			"/home/daniel/Workspace/Sponza/data/skybox/top.jpg",
			"/home/daniel/Workspace/Sponza/data/skybox/bottom.jpg",

			"/home/daniel/Workspace/Sponza/data/skybox/front.jpg",
			"/home/daniel/Workspace/Sponza/data/skybox/back.jpg"
		};

		m_texture = sponza::LoadCubeMap(faces);

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}