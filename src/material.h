#ifndef SPONZA_MATERIAL_H
#define SPONZA_MATERIAL_H

#include <glm/vec3.hpp>
#include <string>

namespace sponza
{
	struct Material
	{
		std::string name;

		std::string ambient_texture;
		std::string diffuse_texture;
		std::string specular_texture;
		std::string alpha_texture;
		std::string displace_texture;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float specular_exponent;
		float optical_density;
		float dissolve;
	};
}

#endif //SPONZA_MATERIAL_H
