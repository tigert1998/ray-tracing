#pragma once

class Material;

#include <boost/optional.hpp>

#include <glm/glm.hpp>

#include "hitable.h"

class Material {
public:
    virtual boost::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray &ray, const HitRecord &record) const = 0;
};
