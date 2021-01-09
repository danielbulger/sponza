#ifndef SPONZA_SHADOW_MAP_H
#define SPONZA_SHADOW_MAP_H

#include <cstdint>
#include <vector>
#include "shader.h"
#include "resource.h"
#include "light.h"
#include "mesh.h"

namespace sponza
{
	class ShadowMap
	{
	public:
		ShadowMap(uint32_t width, uint32_t height);

		~ShadowMap();

		void load(const Resource &resource);

		void updateProjectionMatrix(float near, float far);

		void bindForWrite(const PointLight &light, float farPlane) const;

		[[nodiscard]]
		inline uint32_t getTextureId() const
		{
			return m_texture;
		}

	private:
		glm::mat4 m_projection;
		uint32_t m_texture;
		uint32_t m_frameBuffer;
		uint32_t m_width;
		uint32_t m_height;
		Shader m_shader;
	};
}

#endif
