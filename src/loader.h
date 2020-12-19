#ifndef SPONZA_LOADER_H
#define SPONZA_LOADER_H

#include <memory>
#include <vector>
#include <material.h>
#include <mesh.h>
#include <map>

namespace sponza
{
	std::unique_ptr<std::map<std::string, Material>> LoadMaterial(const char *file);

	std::unique_ptr<std::vector<Mesh>> LoadMesh(const char *file);
}

#endif //SPONZA_LOADER_H
