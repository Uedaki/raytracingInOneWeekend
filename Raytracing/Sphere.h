#pragma once

#include <glm/glm.hpp>

#include "Hitable.h"

class Material;

class Sphere : public Hitable
{
	glm::vec3 _center;
	Material* _material;
	float _radius;

public:
	Sphere() = default;
	Sphere(glm::vec3 center, float radius, Material* material);

	bool hit(const Ray& ray, const float t_min, const float t_max, HitRecord& record) const override;
};