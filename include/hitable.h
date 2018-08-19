#pragma once

struct HitRecord;

#include <boost/optional.hpp>

#include "ray.h"
#include "material.h"

struct HitRecord {
    double t;
    glm::vec3 normal;
    std::weak_ptr<Material> material_ptr;
};

class Hitable {
public:
    virtual boost::optional<HitRecord> Hit(const Ray &ray, std::pair<double, double> t_range) const = 0;
};
