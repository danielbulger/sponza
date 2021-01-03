#include "mesh.h"
#include <glad/glad.h>

namespace sponza
{
	void InitialiseMesh(Mesh &mesh)
	{
		glGenVertexArrays(1, &mesh.vao_id);
		glGenBuffers(1, &mesh.vba_id);
		glGenBuffers(1, &mesh.index_buffer_id);

		glBindVertexArray(mesh.vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vba_id);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(glm::vec3), &mesh.indices[0], GL_STATIC_DRAW);

		// Vertex position attribute.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		// Vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

		// Texture coordinates.
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tex));

		// Clear the set VAO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void CleanupMesh(Mesh &mesh)
	{
		glDeleteVertexArrays(1, &mesh.vao_id);
		glDeleteVertexArrays(1, &mesh.vba_id);
	}
}