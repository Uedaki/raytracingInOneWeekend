#include "Camera.h"

#include "ctmRand.h"

namespace
{
	glm::vec3 random_in_unit_disk()
	{
		glm::vec3 p;
		do
		{
			p = 2.0f * glm::vec3(ctmRand(), ctmRand(), 0) - glm::vec3(1, 1, 0);
		} while (glm::dot(p, p) >= 1);
		return (p);
	}
}

Camera::Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float vfov, float aspect, float aperture, float focus_dist)
	: _origin(lookFrom)
{
	_lens_radius = aperture / 2;

	float theta = glm::radians(vfov);
	float halfHeight = glm::tan(theta / 2);
	float halfWidth = aspect * halfHeight;

	_w = glm::normalize(lookFrom - lookAt);
	_u = glm::normalize(glm::cross(up, _w));
	_v = glm::cross(_w, _u);

	_lowerLeft = _origin - focus_dist * halfWidth * _u - focus_dist * halfHeight * _v - _w;
	_horizontal = focus_dist * 2 * halfWidth * _u;
	_vertical = focus_dist * 2 * halfHeight * _v;
}

Ray Camera::getRay(const float s, const float t) const
{
	glm::vec3 rd = _lens_radius * random_in_unit_disk();
	glm::vec3 offset = _u * rd.x + _v * rd.y;
	return (Ray(_origin, _lowerLeft + s * _horizontal + t * _vertical - _origin));
}