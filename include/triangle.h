#pragma once

#include <array>

#include "hitable.h"

class Triangle: public Hitable {
private:
	std::array<glm::vec3, 3> p_;
	std::shared_ptr<Material> material_ptr_;
public:
	Triangle() = delete;
	Triangle(std::array<glm::vec3, 3> p, std::shared_ptr<Material> material_ptr);
	boost::optional<HitRecord> Hit(const Ray &ray, std::pair<double, double> t_range) const;
};