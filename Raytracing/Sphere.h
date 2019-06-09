#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "Hitable.h"

class Material;

class Sphere : public Hitable
{
	glm::vec3					_center;
	std::shared_ptr<Material>	_material;
	float						_radius;

public:
	Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> material);

	bool hit(const Ray& ray, const float t_min, const float t_max, HitRecord& record) const override;
};