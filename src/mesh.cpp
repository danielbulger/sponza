#include "mesh.h"
#include <glad/glad.h>
#include <iostream>

namespace sponza
{

	void InitialiseMesh(Mesh &mesh)
	{
		glGenVertexArrays(1, &mesh.vao_id);
		glGenBuffers(1, &mesh.vbo_id);
		glGenBuffers(1, &mesh.index_buffer_id);

		glBindVertexArray(mesh.vao_id);

//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer_id);
//		glBufferData(
//			GL_ELEMENT_ARRAY_BUFFER,
//			mesh.indices.size() * sizeof(glm::vec3),
//			&mesh.indices[0],
//			GL_STATIC_DRAW
//		);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo_id);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

		// Vertex position attribute.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(0);

		// Vertex normals
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Tangent coordinates.
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Texture coordinates.
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (9 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// Clear the set VAO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void ComputeTangents(Mesh &mesh)
	{
		for (size_t i = 0; i < mesh.vertices.size(); i += 3)
		{
			glm::vec3 &v0 = mesh.vertices[i].position;
			glm::vec3 &v1 = mesh.vertices[i + 1].position;
			glm::vec3 &v2 = mesh.vertices[i + 2].position;

			glm::vec2 &uv0 = mesh.vertices[i].tex;
			glm::vec2 &uv1 = mesh.vertices[i + 1].tex;
			glm::vec2 &uv2 = mesh.vertices[i + 2].tex;

			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;

			glm::vec2 delta1 = uv1 - uv0;
			glm::vec2 delta2 = uv2 - uv0;

			const float r = 1.0f / (delta1.x * delta2.y - delta1.y * delta2.x);
			glm::vec3 tangent = r * (edge1 * delta2.y - edge2 * delta1.y);

			mesh.vertices[i].tangent += tangent;
			mesh.vertices[i + 1].tangent += tangent;
			mesh.vertices[i + 2].tangent += tangent;
		}

		for(auto &vertex : mesh.vertices) {
			vertex.tangent = glm::normalize(vertex.tangent);
		}
	}

	void CleanupMesh(Mesh &mesh)
	{
		glDeleteVertexArrays(1, &mesh.vao_id);
		glDeleteBuffers(1, &mesh.vbo_id);
//		glDeleteBuffers(1, &mesh.index_buffer_id);

		glDeleteTextures(1, &mesh.material->ambient_texture.id);
		glDeleteTextures(1, &mesh.material->specular_texture.id);
		glDeleteTextures(1, &mesh.material->diffuse_texture.id);
		glDeleteTextures(1, &mesh.material->displace_texture.id);
		glDeleteTextures(1, &mesh.material->alpha_texture.id);
	}
}