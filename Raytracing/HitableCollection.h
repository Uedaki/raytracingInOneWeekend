#pragma once

#include <vector>
#include <memory>
#include "Ray.h"
#include "Hitable.h"

class HitableCollection : public Hitable
{
	std::vector<std::shared_ptr<Hitable>> _collection;

public:
	HitableCollection() = default;
	HitableCollection(const std::vector<std::shared_ptr<Hitable>>& collection)
		: _collection(collection)
	{}

	bool hit(const Ray& ray, const float t_min, const float t_max, HitRecord& record) const override
	{
		HitRecord tmpRecord;
		float closest = t_max;
		bool hasHitAnything = false;
		for (auto& hitable : _collection)
		{
			if (hitable->hit(ray, t_min, closest, tmpRecord))
			{
				hasHitAnything = true;
				closest = tmpRecord.t;
				record = tmpRecord;
			}
		}
		return (hasHitAnything);
	}
};