#ifndef SPONZA_LOADER_H
#define SPONZA_LOADER_H

#include <memory>
#include <vector>
#include <mesh.h>
#include <map>

namespace sponza
{
	std::vector<Mesh> LoadMesh(const char *file);

	uint32_t LoadCubeMap(const std::vector<std::string> &faces);

	Texture LoadTexture(const std::string &name);

	Texture LoadTexture(
		const std::string &base_directory,
		const std::string &name,
		std::map<std::string, Texture> &textures
	);

	std::map<std::string, std::shared_ptr<Material>> LoadMaterial(
		const std::string &base_directory, const std::string &file
	);

}

#endif //SPONZA_LOADER_H
