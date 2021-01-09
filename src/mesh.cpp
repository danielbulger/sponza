#include "mesh.h"
#include <glad/glad.h>
#include <iostream>

namespace sponza
{

	uint32_t InitQuadMesh()
	{
		static float vertices[] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		uint32_t vao, vbo;

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vao);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		// Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);

		// Tex Coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

		glBindVertexArray(0);

		return vao;
	}

	void Render(
		const Mesh &mesh,
		const PointLight &light,
		int depthTexture,
		float farPlane,
		bool updateMaterial,
		bool updateShader
	)
	{
		const Shader &shader = mesh.shader;

		if (updateShader)
		{
			shader.use();

			shader.setMat4("modelMatrix", glm::mat4(1.0f));
			shader.setFloat("farPlane", farPlane);
			shader.setVec4("lightPosition", light.position);
			shader.setVec4("lightAmbient", light.ambient);
			shader.setVec4("lightDiffuse", light.diffuse);
			shader.setVec4("lightSpecular", light.specular);
			shader.setVec4("lightCoef", light.coef);
		}

		if (updateMaterial || updateShader)
		{

			uint32_t boundTexture = 0;

			shader.setVec3("ambient", mesh.material->ambient);
			shader.setVec3("diffuse", mesh.material->diffuse);
			shader.setVec3("specular", mesh.material->specular);
			shader.setFloat("specularExponent", mesh.material->specularExponent);
			shader.setBool("hasSpecular", mesh.material->specularTexture.id > 0);
			shader.setBool("hasAlpha", mesh.material->alphaTexture.id > 0);

			for(int i = 0; i < 5; ++i)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glActiveTexture(GL_TEXTURE0 + boundTexture);
			glBindTexture(GL_TEXTURE_2D, mesh.material->ambientTexture.id);
			shader.setInt("ambientTexture", boundTexture++);

			glActiveTexture(GL_TEXTURE0 + boundTexture);
			glBindTexture(GL_TEXTURE_2D, mesh.material->diffuseTexture.id);
			shader.setInt("diffuseTexture", boundTexture++);

			if(mesh.material->specularTexture.id > 0)
			{
				glActiveTexture(GL_TEXTURE0 + boundTexture);
				glBindTexture(GL_TEXTURE_2D, mesh.material->specularTexture.id);
				shader.setInt("specularTexture", boundTexture++);
			}
			else
			{
				shader.setInt("specularTexture", 0);
			}

			if(mesh.material->alphaTexture.id > 0)
			{
				glActiveTexture(GL_TEXTURE0 + boundTexture);
				glBindTexture(GL_TEXTURE_2D, mesh.material->alphaTexture.id);
				shader.setInt("alphaTexture",  boundTexture++);
			}
			else
			{
				shader.setInt("alphaTexture",  0);
			}

			// Is there a normal map attached to the mesh
			if (mesh.material->displaceTexture.id > 0)
			{
				glActiveTexture(GL_TEXTURE0 + boundTexture);
				glBindTexture(GL_TEXTURE_2D, mesh.material->displaceTexture.id);
				shader.setInt("normalTexture", boundTexture++);
			}
			else
			{
				shader.setInt("normalTexture", 0);
			}

			glActiveTexture(GL_TEXTURE0 + boundTexture);
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexture);
			shader.setInt("depthTexture",  boundTexture);
		}

		DrawMesh(mesh);
	}

	void DrawMesh(const Mesh &mesh)
	{
		glBindVertexArray(mesh.vaoId);

		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
	}

	void InitialiseMesh(Mesh &mesh, Shader defaultShader, Shader normalMapShader)
	{

		ComputeTangents(mesh);

		if (mesh.material->displaceTexture.id > 0)
		{
			mesh.shader = normalMapShader;
		}
		else
		{
			mesh.shader = defaultShader;
		}

		glGenVertexArrays(1, &mesh.vaoId);
		glGenBuffers(1, &mesh.vboId);
		glGenBuffers(1, &mesh.indexBufferId);

		glBindVertexArray(mesh.vaoId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), &mesh.indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vboId);
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
		for (size_t i = 0; i < mesh.indices.size(); i += 3)
		{

			const glm::vec3 &v0 = mesh.vertices[mesh.indices[i]].position;
			const glm::vec3 &v1 = mesh.vertices[mesh.indices[i + 1]].position;
			const glm::vec3 &v2 = mesh.vertices[mesh.indices[i + 2]].position;

			const glm::vec2 &uv0 = mesh.vertices[mesh.indices[i]].tex;
			const glm::vec2 &uv1 = mesh.vertices[mesh.indices[i + 1]].tex;
			const glm::vec2 &uv2 = mesh.vertices[mesh.indices[i + 2]].tex;

			const glm::vec3 edge1 = v1 - v0;
			const glm::vec3 edge2 = v2 - v0;

			const glm::vec2 delta1 = uv1 - uv0;
			const glm::vec2 delta2 = uv2 - uv0;

			const float r = 1.0f / (delta1.x * delta2.y - delta1.y * delta2.x);
			const glm::vec3 tangent = r * (edge1 * delta2.y - edge2 * delta1.y);

			mesh.vertices[mesh.indices[i]].tangent += tangent;
			mesh.vertices[mesh.indices[i + 1]].tangent += tangent;
			mesh.vertices[mesh.indices[i + 2]].tangent += tangent;
		}

		for (auto &vertex : mesh.vertices)
		{
			vertex.tangent = glm::normalize(vertex.tangent);
		}
	}

	void CleanupMesh(Mesh &mesh)
	{
		glDeleteVertexArrays(1, &mesh.vaoId);
		glDeleteBuffers(1, &mesh.vboId);
		glDeleteBuffers(1, &mesh.indexBufferId);

		glDeleteTextures(1, &mesh.material->ambientTexture.id);
		glDeleteTextures(1, &mesh.material->specularTexture.id);
		glDeleteTextures(1, &mesh.material->diffuseTexture.id);
		glDeleteTextures(1, &mesh.material->displaceTexture.id);
		glDeleteTextures(1, &mesh.material->alphaTexture.id);
	}
}