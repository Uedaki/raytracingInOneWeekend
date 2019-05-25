#pragma once

#include <glm/glm.hpp>

#include "Ray.h"
#include "Hitable.h"

glm::vec3 randomInUnitSphere()
{
	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3((static_cast<float>(std::rand() % 100) / 100),
			(static_cast<float>(std::rand() % 100) / 100),
			(static_cast<float>(std::rand() % 100) / 100)) - glm::vec3(1, 1, 1);
	} while (glm::length(p) >= 1);
	return (p);
}

class Material
{
public:
	virtual	bool scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambert : public Material
{
	glm::vec3 _albedo;

public:
	Lambert(const glm::vec3& albedo)
		: _albedo(albedo)
	{}

	bool scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override
	{
		glm::vec3 target = hit.p + hit.normal + randomInUnitSphere();
		scattered = Ray(hit.p, target - hit.p);
		attenuation = _albedo;
		return (true);
	}
};

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
{
	return (v - 2 * glm::dot(v, n) * n);
}

class Metal : public Material
{
	glm::vec3 _albedo;
	float _fuzz;
public:
	Metal(const glm::vec3& albedo, const float fuzz)
		: _albedo(albedo), _fuzz(fuzz)
	{
		if (_fuzz >= 1)
			_fuzz = 1;
	}

	bool scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override
	{
		glm::vec3 reflected = reflect(glm::normalize(in.direction()), hit.normal);
		scattered = Ray(hit.p, reflected + _fuzz * randomInUnitSphere());
		attenuation = _albedo;
		return (glm::dot(scattered.direction(), hit.normal) > 0);
	}
};

bool refract(const glm::vec3& v, const glm::vec3& n, const float ni_over_nt, glm::vec3& refracted)
{
	glm::vec3 uv = glm::normalize(v);
	float dt = glm::dot(uv, n);
	float discriminant = 1 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return (true);
	}
	return (false);
}

float schlick(const float cosine, const float ri)
{
	float r0 = (1 - ri) / (1 + ri);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow(1 - cosine, 5));
}

class Dialectric : public Material
{
	float _ri;

public:
	Dialectric(const float ri)
		: _ri(ri)
	{}

	bool scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override
	{
		glm::vec3 outwardNormal;
		glm::vec3 reflected = reflect(in.direction(), hit.normal);
		float ni_over_nt;
		attenuation = glm::vec3(1, 1, 1);
		glm::vec3 refracted;
		float reflectProb;
		float cosine;
		if (glm::dot(in.direction(), hit.normal) > 0)
		{
			outwardNormal = -hit.normal;
			ni_over_nt = _ri;
			cosine = _ri * glm::dot(in.direction(), hit.normal) / in.direction().length();
		}
		else
		{
			outwardNormal = hit.normal;
			ni_over_nt = 1 / _ri;
			cosine = -glm::dot(in.direction(), hit.normal) / in.direction().length();
		}
		if (refract(in.direction(), outwardNormal, ni_over_nt, refracted))
			reflectProb = schlick(cosine, _ri);
		else
		{
			scattered = Ray(hit.p, refracted);
			reflectProb = 1;
		}
		if (static_cast<float>(std::rand() % 100) / 100 < reflectProb)
			scattered = Ray(hit.p, reflected);
		else
			scattered = Ray(hit.p, refracted);
		return (true);
	}
};