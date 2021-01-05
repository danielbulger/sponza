#include <loader.h>

#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <set>

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wsign-compare"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#pragma GCC diagnostic pop

#include <glad/glad.h>

namespace sponza
{
	void ParseFloat(std::istringstream &stream, float &value)
	{
		std::string temp;

		stream >> temp;

		value = static_cast<float>(std::stod(temp));
	}

	void ParseString(std::istream &stream, std::string &value)
	{
		stream >> value;
	}

	void ParseVec3(std::istream &stream, glm::vec3 &vector)
	{
		std::string temp;

		for (int i = 0; i < 3; ++i)
		{
			stream >> temp;

			vector[i] = static_cast<float>(std::stod(temp));
		}
	}

	void ParseVec2(std::istream &stream, glm::vec2 &vector)
	{
		std::string temp;

		for (int i = 0; i < 2; ++i)
		{
			stream >> temp;

			vector[i] = static_cast<float>(std::stod(temp));
		}
	}

	std::vector<int32_t> SplitFace(std::string &str)
	{
		std::vector<int32_t> result;

		std::istringstream iss(str);
		std::string token;

		while (std::getline(iss, token, '/'))
		{
			result.emplace_back(std::stoi(token));
		}

		return result;
	}

	void GenerateVertices(
		std::vector<Vertex> &vertices,
		const std::vector<glm::vec3> &positions,
		const std::vector<glm::vec3> &normals,
		const std::vector<glm::vec2> &tex,
		std::istringstream &stream
	)
	{
		std::string data;

		while (stream.good())
		{
			Vertex vertex = {};
			stream >> data;
			std::vector<int32_t> face = SplitFace(data);

			// -1 as obj files index start at 1 not 0.
			vertex.position = positions[face[0] - 1];
			vertex.tex = tex[face[1] - 1];
			vertex.normal = normals[face[2] - 1];

			vertices.emplace_back(vertex);
		}
	}

	std::vector<Mesh> LoadMesh(const Resource &resource, const char *file)
	{
		std::vector<Mesh> meshes;

		std::ifstream in(resource.getFilePath(file));

		// File couldn't be open for what ever reason, return empty list.
		if (!in.is_open())
		{
			return meshes;
		}

		const std::string base_directory = Resource::getDirectory(file);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> tex;

		std::string line;
		std::string property;

		std::map<std::string, std::shared_ptr<Material>> materials;

		Mesh mesh = {};

		bool listening = false;

		while (std::getline(in, line))
		{
			// If the line is empty or begins with a comment, ignore
			if (line.empty() || line[0] == '#')
			{
				continue;
			}

			std::istringstream stream(line);

			stream >> property;

			if (property == "O" || property == "g")
			{
				if (!listening)
				{
					listening = true;
				}
				else
				{
					meshes.emplace_back(mesh);

					mesh = {};
				}

				ParseString(stream, mesh.name);
			}
			else if (property == "v")
			{
				glm::vec3 position;
				ParseVec3(stream, position);
				positions.emplace_back(position);
			}
			else if (property == "vt")
			{
				glm::vec2 vtex;
				ParseVec2(stream, vtex);
				tex.emplace_back(vtex);
			}
			else if (property == "vn")
			{
				glm::vec3 normal;
				ParseVec3(stream, normal);
				normals.emplace_back(normal);
			}
			else if (property == "f")
			{
				GenerateVertices(
					mesh.vertices, positions, normals, tex, stream
				);
			}
			else if (property == "mtllib")
			{
				std::string path;

				stream >> path;
				ParseString(stream, path);

				materials = LoadMaterial(resource, base_directory, path);
			}
			else if (property == "usemtl")
			{
				std::string material_name;
				ParseString(stream, material_name);

				auto material = materials.find(material_name);

				if (material != materials.end())
				{
					mesh.material = material->second;
				}
				else
				{
					mesh.material = nullptr;
				}
			}
		}

		in.close();

		return meshes;
	}

	std::map<std::string, std::shared_ptr<Material>> LoadMaterial(
		const Resource &resource,
		const std::string &base_directory,
		const std::string &file
	)
	{
		std::map<std::string, std::shared_ptr<Material>> materials;

		std::map<std::string, Texture> textures;

		const std::string path = resource.getFilePath(base_directory + file);

		std::ifstream in(path);

		// File couldn't be open for what ever reason, return empty list.
		if (!in.is_open())
		{
			return materials;
		}

		std::string line;

		std::string property;

		Material material = {};

		bool listening = false;

		while (std::getline(in, line))
		{
			// If the line is empty or begins with a comment, ignore it.
			if (line.empty() || line[0] == '#')
			{
				continue;
			}

			std::istringstream stream(line);

			stream >> property;

			if (property == "newmtl")
			{
				if (!listening)
				{
					listening = true;
				}
				else
				{
					materials.insert(
						std::pair<std::string, std::shared_ptr<Material>>(
							material.name, std::make_shared<Material>(material)
						)
					);

					material = {};
				}

				ParseString(stream, material.name);
			}
			else if (property == "Ka")
			{
				ParseVec3(stream, material.ambient);
			}
			else if (property == "Kd")
			{
				ParseVec3(stream, material.diffuse);
			}
			else if (property == "Ks")
			{
				ParseVec3(stream, material.specular);
			}
			else if (property == "Ns")
			{
				ParseFloat(stream, material.specular_exponent);
			}
			else if (property == "Ni")
			{
				ParseFloat(stream, material.optical_density);
			}
			else if (property == "d")
			{
				ParseFloat(stream, material.dissolve);
			}
			else if (property == "map_Ka")
			{
				std::string texture_name;
				ParseString(stream, texture_name);
				material.ambient_texture = LoadTexture(resource, base_directory, texture_name, textures);
			}
			else if (property == "map_Kd")
			{
				std::string texture_name;
				ParseString(stream, texture_name);
				material.diffuse_texture = LoadTexture(resource, base_directory, texture_name, textures);
			}
			else if (property == "map_Ks")
			{
				std::string texture_name;
				ParseString(stream, texture_name);
				material.specular_texture = LoadTexture(resource, base_directory, texture_name, textures);
			}
			else if (property == "map_d")
			{
				std::string texture_name;
				ParseString(stream, texture_name);
				material.alpha_texture = LoadTexture(resource, base_directory, texture_name, textures);
			}
			else if (property == "map_Disp")
			{
				std::string texture_name;
				ParseString(stream, texture_name);
				material.displace_texture = LoadTexture(resource, base_directory, texture_name, textures);
			}
		}

		// Add the last remaining material in.
		materials.insert(
			std::pair<std::string, std::shared_ptr<Material>>(
				material.name, std::make_shared<Material>(material)
			));

		in.close();

		return materials;
	}

	Texture LoadTexture(
		const Resource &resource,
		const std::string &base_directory,
		const std::string &name,
		std::map<std::string, Texture> &textures
	)
	{
		if (!textures.empty())
		{
			auto texture = textures.find(name);

			if (texture != textures.end())
			{
				return texture->second;
			}
		}

		const std::string path = resource.getFilePath(base_directory + name);

		Texture texture = LoadTexture(path);

		if (texture.id != 0)
		{
			textures.insert(
				std::pair<std::string, Texture>(
					name, texture
				));
		}

		return texture;
	}

	Texture LoadTexture(const std::string &name)
	{
		Texture new_texture = {
			.id = 0,
			.width = 0,
			.height = 0,
			.channels = 0
		};

		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(
			name.c_str(),
			&new_texture.width,
			&new_texture.height,
			&new_texture.channels,
			0
		);

		if (data)
		{
			glGenTextures(1, &new_texture.id);
			glBindTexture(GL_TEXTURE_2D, new_texture.id);

			const int format = new_texture.channels == 3 ? GL_RGB : GL_RGBA;

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				format,
				new_texture.width,
				new_texture.height,
				0,
				format,
				GL_UNSIGNED_BYTE,
				data
			);

			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);

			glBindTexture(GL_TEXTURE_2D, 0);

			// Since it has been allocated to the GPU this can now be cleaned up.
			stbi_image_free(data);

			return new_texture;
		}
		else
		{
			return new_texture;
		}
	}

	uint32_t LoadCubeMap(const std::vector<std::string> &faces)
	{
		uint32_t id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		int width, height, channels;

		stbi_set_flip_vertically_on_load(false);

		for (size_t i = 0; i < faces.size(); ++i)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

			if (data)
			{
				const int format = channels == 3 ? GL_RGB : GL_RGBA;

				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					format,
					width,
					height,
					0,
					format,
					GL_UNSIGNED_BYTE,
					data
				);
			}
			else
			{
				std::cout << "Unable to load " << faces[i] << std::endl;
			}

			stbi_image_free(data);

		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return id;
	}
}