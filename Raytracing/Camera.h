#pragma once

#include <glm/glm.hpp>

#include "Ray.h"

class Camera
{
	glm::vec3 _origin;
	glm::vec3 _lowerLeft;
	glm::vec3 _horizontal;
	glm::vec3 _vertical;

	glm::vec3 _w;
	glm::vec3 _u;
	glm::vec3 _v;

	float _lens_radius;

public:
	Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float vfov, float aspect, float aperture, float focus_dist);

	Ray getRay(const float s, const float t) const;
};