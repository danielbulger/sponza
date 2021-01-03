#ifndef SPONZA_MESH_H
#define SPONZA_MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

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
		Material *material;

		uint32_t vao_id;
		uint32_t vba_id;
		uint32_t index_buffer_id;
	};

	void InitialiseMesh(Mesh &mesh);

	void CleanupMesh(Mesh &mesh);
}

#endif //SPONZA_MESH_H
