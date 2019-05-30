#pragma once

class Ray;
class Material;
struct HitRecord;

class Hitable
{
public:
	virtual bool hit(const Ray& ray, const float t_min, const float t_max, HitRecord& record) const = 0;
};