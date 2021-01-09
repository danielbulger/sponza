#ifndef SPONZA_UNIFORMS_H
#define SPONZA_UNIFORMS_H

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "light.h"
#include "camera.h"

namespace sponza
{

	struct GlobalUniforms
	{
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 normalMatrix;
	};

	class Uniforms
	{
	public:
		Uniforms(int shaderId, int normalMapShaderId, const Camera &camera);

		~Uniforms();

		void update(const Camera &camera);
	private:
		uint32_t m_uniformBuffer[1];
		GlobalUniforms m_uniforms;
	};
}

#endif
