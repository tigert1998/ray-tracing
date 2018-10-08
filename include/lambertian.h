#pragma once

#include "material.h"
#include "texture.h"

#include <functional>
#include <memory>

class Lambertian: public Material {
public:
    Lambertian(std::function<double()> dice, std::shared_ptr<Texture> texture_ptr);
    boost::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray &ray, const HitRecord &record) const;

private:
    std::function<double()> dice_;
    std::shared_ptr<Texture> texture_ptr_;
};
