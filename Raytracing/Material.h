#pragma once

#include <glm/glm.hpp>

#include "Ray.h"
#include "Hitable.h"

class Material
{
public:
	virtual	bool scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambert : public Material
{
	glm::vec3 _albedo;

public:
	Lambert(const glm::vec3& albedo);
	
	bool scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override;
};

class Metal : public Material
{
	glm::vec3 _albedo;
	float _fuzz;

public:
	Metal(const glm::vec3& albedo, const float fuzz);

	bool scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override;
};

class Dialectric : public Material
{
	float _ri;

public:
	Dialectric(const float ri);

	bool scatter(const Ray& in, const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override;
};