#pragma once

#include "hitable_list.h"

#include <array>

class Parallelogram: public Hitable {
private:
	HitableList list_;

public:
	Parallelogram() = delete;
	Parallelogram(std::array<glm::vec3, 3> p, std::shared_ptr<Material> material_ptr);
	boost::optional<HitRecord> Hit(const Ray &ray, std::pair<double, double> t_range) const;
};