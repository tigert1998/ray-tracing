#include "noise_texture.h"
#include "perlin_noise.h"

using glm::vec3;

NoiseTexture::NoiseTexture(double scale): scale_(scale) {}

glm::vec3 NoiseTexture::Value(double u, double v, const glm::vec3 &p) const {
	return vec3(generator_.Noise(p * vec3(scale_)) * 0.5 + 0.5);
}

const PerlinNoise NoiseTexture::generator_ = PerlinNoise(256);