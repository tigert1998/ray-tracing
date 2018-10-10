#pragma once

class Material;

#include <boost/optional.hpp>

#include <glm/glm.hpp>

#include "hitable.h"

class Material {
public:
    virtual boost::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray &ray, const HitRecord &record) const = 0;
    virtual glm::vec3 Emit(double u, double v, const glm::vec3 &p) const;
};
