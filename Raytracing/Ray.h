#pragma once

#include <glm/glm.hpp>

class Ray
{
	glm::vec3 _origin;
	glm::vec3 _direction;

public:
	Ray() = default;
	Ray(const glm::vec3& newOrigin, const glm::vec3& newDirection)
		: _origin(newOrigin), _direction(newDirection)
	{}

	const glm::vec3& origin() const { return (_origin); }
	const glm::vec3& direction() const { return (_direction); }

	glm::vec3 pointAtTime(float t) const { return (_origin + t * _direction); }
};