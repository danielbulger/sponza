#ifndef SPONZA_HDR_H
#define SPONZA_HDR_H

#include <cstdint>
#include "shader.h"
#include "resource.h"

namespace sponza
{
	class HDR
	{
	public:
		HDR(int width, int height);

		~HDR();

		void load(const Resource &resource);

		void bindForWrite(uint32_t width, uint32_t height, bool firstPass);

		void render();
	private:
		Shader m_shader;
		uint32_t m_frameBuffer;
		uint32_t m_colourBuffer;
		uint32_t m_renderBuffer;
		uint32_t m_quadVao;
	};
}

#endif
