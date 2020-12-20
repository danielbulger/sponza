#include <loader.h>

#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

namespace sponza
{

	std::string GetDirectory(const char *file)
	{
		const char *slash = strrchr(file, '/');

		if (slash == nullptr)
		{
			return std::string();
		}

		// +1 as we want to include the slash.
		size_t length = slash - file + 1;

		return std::string(file, length);
	}

	void ParseFloat(std::istringstream &stream, float &value)
	{
		std::string temp;

		stream >> temp;

		value = std::stof(temp);
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

			vector[i] = std::stof(temp);
		}
	}

	void ParseVec2(std::istream &stream, glm::vec2 &vector)
	{
		std::string temp;

		for (int i = 0; i < 2; ++i)
		{
			stream >> temp;

			vector[i] = std::stof(temp);
		}
	}

	std::vector<int> SplitFace(std::string &str)
	{
		std::vector<int> result;

		std::istringstream iss(str);
		std::string token;

		while (std::getline(iss, token, '/'))
		{
			result.push_back(std::stoi(token));
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

		for(int i = 0; stream.good(); ++i)
		{
			Vertex vertex = {};
			stream >> data;
			std::vector<int> face = SplitFace(data);

			// -1 as obj files index start at 1 not 0.
			vertex.position = positions[face[0] - 1];
			vertex.tex = tex[face[1] - 1];
			vertex.normal = normals[face[2] - 1];

			vertices.push_back(vertex);
		}
	}

	std::unique_ptr<std::vector<Mesh>> LoadMesh(const char *file)
	{
		auto meshes = std::make_unique<std::vector<Mesh>>();

		std::ifstream in(file);

		// File couldn't be open for what ever reason, return empty list.
		if (!in.is_open())
		{
			return meshes;
		}

		const std::string base_directory = GetDirectory(file);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> tex;

		std::string line;
		std::string property;

		std::unique_ptr<std::map<std::string, Material>> materials;

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
					meshes->push_back(mesh);

					mesh = {};
				}

				ParseString(stream, mesh.name);
			}
			else if (property == "v")
			{
				glm::vec3 position;
				ParseVec3(stream, position);
				positions.push_back(position);
			}
			else if (property == "vt")
			{
				glm::vec2 vtex;
				ParseVec2(stream, vtex);
				tex.push_back(vtex);
			}
			else if (property == "vn")
			{
				glm::vec3 normal;
				ParseVec3(stream, normal);
				normals.push_back(normal);
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

				path = base_directory + path;

				materials = LoadMaterial(path.c_str());
			}
			else if (property == "usemtl")
			{
				std::string material_name;
				ParseString(stream, material_name);

				auto material = materials->find(material_name);

				if (material != materials->end())
				{
					mesh.material = material->second;
				}
			}
		}

		in.close();

		return meshes;
	}

	std::unique_ptr<std::map<std::string, Material>> LoadMaterial(const char *file)
	{
		auto materials = std::make_unique<std::map<std::string, Material>>();

		std::ifstream in(file);

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
					materials->insert(
						std::pair<std::string, Material>(
							material.name, material
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
				ParseString(stream, material.ambient_texture);
			}
			else if (property == "map_Kd")
			{
				ParseString(stream, material.diffuse_texture);
			}
			else if (property == "map_Ks")
			{
				ParseString(stream, material.specular_texture);
			}
			else if (property == "map_a")
			{
				ParseString(stream, material.alpha_texture);
			}
			else if (property == "map_Disp")
			{
				ParseString(stream, material.displace_texture);
			}
		}

		// Add the last remaining material in.
		materials->insert(
			std::pair<std::string, Material>(
				material.name, material
			));

		in.close();

		return materials;
	}
}