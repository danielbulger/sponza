#ifndef SPONZA_MESH_H
#define SPONZA_MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

namespace sponza
{

	struct Texture
	{
		uint32_t id;
		int32_t width;
		int32_t height;
		int32_t channels;
	};

	struct Material
	{
		std::string name;

		Texture ambient_texture;
		Texture diffuse_texture;
		Texture specular_texture;
		Texture alpha_texture;
		Texture displace_texture;

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
		glm::vec3 tangent;
		glm::vec2 tex;
	};

	struct Mesh
	{
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::shared_ptr<Material> material;

		uint32_t vao_id;
		uint32_t vbo_id;
		uint32_t index_buffer_id;
	};

	void InitialiseMesh(Mesh &mesh);

	void ComputeTangents(Mesh &mesh);

	void CleanupMesh(Mesh &mesh);
}

#endif //SPONZA_MESH_H
