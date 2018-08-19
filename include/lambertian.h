#pragma once

#include "material.h"

#include <functional>

class Lambertian: public Material {
public:
    Lambertian(std::function<double()> dice, glm::vec3 albedo);
    boost::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray &ray, const HitRecord &record) const;

private:
    std::function<double()> dice_;
    glm::vec3 albedo_;
    glm::vec3 RandomInUnitSphere() const;
};
