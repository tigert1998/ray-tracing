#pragma once

#include "texture.h"
#include "perlin_noise.h"

class NoiseTexture: public Texture {
private:
	double scale_;
	const static PerlinNoise generator_;

public:
	NoiseTexture() = delete;
	NoiseTexture(double scale);
	glm::vec3 Value(double u, double v, const glm::vec3 &p) const;
};