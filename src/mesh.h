#ifndef SPONZA_MESH_H
#define SPONZA_MESH_H

#include <material.h>
#include <glm/glm.hpp>

namespace sponza
{

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex;
	};

	struct Mesh
	{
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		Material material;
	};
}

#endif //SPONZA_MESH_H
