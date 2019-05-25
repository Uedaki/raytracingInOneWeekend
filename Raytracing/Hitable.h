#pragma once

#include <glm/glm.hpp>
#include "Ray.h"

class Material;

struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	Material* material;
};

class Hitable
{
public:
	virtual bool hit(const Ray& ray, const float t_min, const float t_max, HitRecord& record) const = 0;
};