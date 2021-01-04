
#ifndef SPONZA_SKYBOX_H
#define SPONZA_SKYBOX_H


#include "shader.h"
#include "camera.h"

namespace sponza
{
	class Skybox
	{
	public:

		Skybox() : m_texture(0), m_buffer(0)
		{}

		~Skybox();

		void render(const Camera &camera, const glm::mat4 &projection);

		void load();

	private:
		Shader m_shader;

		uint32_t m_texture;
		uint32_t m_vao;
		uint32_t m_buffer;
	};
}

#endif
