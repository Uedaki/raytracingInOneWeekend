#pragma once

#include <glm/glm.hpp>
#include "Hitable.h"
#include "Material.h"

class Sphere : public Hitable
{
	glm::vec3 _center;
	Material* _material;
	float _radius;

public:
	Sphere() = default;
	Sphere(glm::vec3 center, float radius, Material* material)
		: _center(center), _radius(radius), _material(material)
	{}

	bool hit(const Ray& ray, const float t_min, const float t_max, HitRecord& record) const override
	{
		record.material = _material;
		glm::vec3 oc = ray.origin() - _center;
		float a = glm::dot(ray.direction(), ray.direction());
		float b = glm::dot(oc, ray.direction());
		float c = glm::dot(oc, oc) - _radius * _radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0)
		{
			float temp1 = (-b - sqrt(discriminant)) / a;
			float temp2 = (-b + sqrt(discriminant)) / a;
			if (t_min < temp1 && temp1 < t_max)
			{
				record.t = temp1;
				record.p = ray.pointAtTime(temp1);
				record.normal = (record.p - _center) / _radius;
				return (true);
			}
			else if (t_min < temp2 && temp2 < t_max)
			{
				record.t = temp2;
				record.p = ray.pointAtTime(temp2);
				record.normal = (record.p - _center) / _radius;
				return (true);
			}
			
		}
		return (false);
	}
};