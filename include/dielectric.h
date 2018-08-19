#pragma once

#include "material.h"

class Dielectric: public Material {
public:
    Dielectric() = delete;
    Dielectric(glm::vec3 attenuation, double refractive_index);
    boost::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray &ray, const HitRecord &record) const;

private:
    glm::vec3 attenuation_;
    double refractive_index_;
};
