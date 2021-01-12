#ifndef SPONZA_BOUNDING_VOLUME_H
#define SPONZA_BOUNDING_VOLUME_H

#include <glm/glm.hpp>
#include <vector>
#include <mesh.h>

namespace sponza
{
	struct AABB
	{
		glm::vec3 min;
		glm::vec3 max;
	};

	AABB CalculateAABB(const Mesh &mesh);

	std::vector<glm::vec4> CalculateSpheres(const std::vector<Mesh> &meshes);

	void Cull(
		const glm::vec4 planes[6],
		const std::vector<glm::vec4> &spheres,
		const std::vector<Mesh> &meshes,
		std::vector<const Mesh*> &visibleMeshes
	);
}

#endif
