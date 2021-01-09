#ifndef SPONZA_MESH_H
#define SPONZA_MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "shader.h"
#include "light.h"

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

		Texture ambientTexture;
		Texture diffuseTexture;
		Texture specularTexture;
		Texture alphaTexture;
		Texture displaceTexture;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float specularExponent;
		float opticalDensity;
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

		uint32_t vaoId;
		uint32_t vboId;
		uint32_t indexBufferId;

		Shader shader;
	};

	uint32_t InitQuadMesh();

	void InitialiseMesh(Mesh &mesh, Shader defaultShader, Shader normalMapShader);

	void ComputeTangents(Mesh &mesh);

	void Render(
		const Mesh &mesh,
		const PointLight &light,
		int depthTexture,
		float farPlane,
		bool updateMaterial,
		bool updateShader
	);

	void DrawMesh(const Mesh &mesh);

	void CleanupMesh(Mesh &mesh);
}

#endif //SPONZA_MESH_H
