#pragma once

#include <glm/glm.hpp>

#include <memory>

class Material;

struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	std::shared_ptr<Material> material;
};