#ifndef SPONZA_LIGHT_H
#define SPONZA_LIGHT_H

#include <glm/glm.hpp>

namespace sponza
{
	struct PointLight
	{
		glm::vec4 position;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		glm::vec4 coef;
	};

	void InitialiseLights(sponza::PointLight lights[2]);
}

#endif
