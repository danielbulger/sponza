#include "mesh.h"
#include <glad/glad.h>

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

		// Texture coordinates.
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Clear the set VAO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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