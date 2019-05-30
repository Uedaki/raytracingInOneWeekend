#pragma once

#include <vector>
#include <memory>

#include "Hitable.h"
#include "Ray.h"

class HitableCollection : public Hitable
{
	std::vector<std::shared_ptr<Hitable>> _collection;

public:
	HitableCollection(const std::vector<std::shared_ptr<Hitable>>& collection);
	bool hit(const Ray& ray, const float t_min, const float t_max, HitRecord& record) const override;
};