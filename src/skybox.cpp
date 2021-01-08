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

		m_shader.setMat4("projectionMatrix", projection);
		m_shader.setMat4("viewMatrix", glm::mat4(glm::mat3(camera.getViewMatrix())));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
		m_shader.setInt("skybox", 0);

		glBindVertexArray(m_vao);

		glDrawArrays(GL_TRIANGLES, 0, sizeof(VERTICES) / 3);

		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
	}

	void Skybox::load(const Resource &resource)
	{
		m_shader = Shader(
			resource.getFilePath("/skybox.vert").c_str(),
			resource.getFilePath("/skybox.frag").c_str()
		);

		const std::vector<std::string> faces = {
			resource.getFilePath("/skybox/right.jpg"),
			resource.getFilePath("/skybox/left.jpg"),

			resource.getFilePath("/skybox/top.jpg"),
			resource.getFilePath("/skybox/bottom.jpg"),

			resource.getFilePath("/skybox/front.jpg"),
			resource.getFilePath("/skybox/back.jpg")
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