#include "diffuse_light.h"

using boost::optional, boost::none;
using std::pair, std::shared_ptr;
using namespace glm;

optional<pair<vec3, Ray>> DiffuseLight::Scatter(const Ray &ray, const HitRecord &record) const {
	return none;
}

vec3 DiffuseLight::Emit(double u, double v, const vec3 &p) const {
	return texture_ptr_->Value(u, v, p);
}

DiffuseLight::DiffuseLight(shared_ptr<Texture> texture_ptr) {
    texture_ptr_ = texture_ptr;
}