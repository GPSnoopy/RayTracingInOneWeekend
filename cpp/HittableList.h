#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList final : public Hittable
{
public:

	size_t size() const { return list_.size(); }

	void Add(const Hittable* hittable)
	{
		Add(std::unique_ptr<const Hittable>(hittable));
	}

	void Add(std::unique_ptr<const Hittable>&& hittable)
	{
		list_.push_back(std::move(hittable));
	}

	std::optional<HitRecord> Hit(const Ray& ray, const float tMin, float tMax) const override
	{
		std::optional<HitRecord> record;

		for (const auto& hittable : list_)
		{
			if (const auto rec = hittable->Hit(ray, tMin, tMax))
			{
				tMax = rec->t;
				record = rec;
			}
		}

		return record;
	}

private:

	std::vector<std::unique_ptr<const Hittable>> list_;
};