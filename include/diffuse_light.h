#pragma once

#include "material.h"
#include "texture"

class DiffuseLight: public Material {
private:
	std::shared_ptr<Texture> texture_ptr_;

public:
	DiffuseLight() = delete;
	DiffuseLight(std::shared_ptr<Texture> texture_ptr);
	boost::optional<std::pair<glm::vec3, Ray>> Scatter(const Ray &ray, const HitRecord &record) const;
    glm::vec3 Emit(double u, double v, const glm::vec3 &p) const;
};