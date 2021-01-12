#include <bounding_volume.h>
#include <cfloat>

namespace sponza
{
	AABB CalculateAABB(const Mesh &mesh)
	{
		AABB box = {};
		box.min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		box.max = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);

		for (const auto &vertex : mesh.vertices)
		{
			for (int i = 0; i < glm::vec3::length(); ++i)
			{
				if (vertex.position[i] < box.min[i])
				{
					box.min[i] = vertex.position[i];
				}

				if (vertex.position[i] > box.max[i])
				{
					box.max[i] = vertex.position[i];
				}
			}
		}

		return box;
	}

	std::vector<glm::vec4> CalculateSpheres(const std::vector<Mesh> &meshes)
	{
		std::vector<glm::vec4> spheres;
		spheres.reserve(meshes.size());

		for (const auto &mesh : meshes)
		{
			const AABB box = CalculateAABB(mesh);

			const glm::vec3 center = (box.max + box.min) / 2.0f;

			double radius = 0;

			for (const auto &vertex : mesh.vertices)
			{
				radius = fmax(radius, (double) glm::distance(vertex.position, center));
			}

			spheres.emplace_back(glm::vec4(center.x, center.y, center.z, (float) radius));
		}

		return spheres;
	}

	void Cull(
		const glm::vec4 planes[6],
		const std::vector<glm::vec4> &spheres,
		const std::vector<Mesh> &meshes,
		std::vector<const Mesh *> &visibleMeshes
	)
	{
		visibleMeshes.clear();

		for (size_t i = 0; i < spheres.size(); ++i)
		{
			const glm::vec4 &sphere = spheres[i];

			bool within = true;

			for (int p = 0; p < 6; ++p)
			{
				if (glm::dot(planes[p], glm::vec4(sphere.x, sphere.y, sphere.z, 1.0f)) < -sphere.w)
				{
					within = false;
					break;
				}
			}

			if (within)
			{
				visibleMeshes.push_back(&meshes[i]);
			}
		}
	}
}