#include "HitableCollection.h"

#include "HitRecord.h"

HitableCollection::HitableCollection(const std::vector<std::shared_ptr<Hitable>>& collection)
	: _collection(collection)
{}

bool HitableCollection::hit(const Ray& ray, const float t_min, const float t_max, HitRecord& record) const
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