#pragma once

#include "material.h"

class Metal: public Material {
public:
    Metal() = delete;
    Metal(glm::vec3 albedo);
    boost::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray &ray, const HitRecord &record) const;

private:
    glm::vec3 albedo_;
};
