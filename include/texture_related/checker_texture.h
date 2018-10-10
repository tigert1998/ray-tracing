#pragma once

#include "texture.h"

#include <memory>

class CheckerTexture: public Texture {
private:
	std::shared_ptr<Texture> odd_ptr_, even_ptr_;

public:
	CheckerTexture() = delete;
	CheckerTexture(std::shared_ptr<Texture> odd_ptr, std::shared_ptr<Texture> even_ptr);
	glm::vec3 Value(double u, double v, const glm::vec3 &p) const;
};