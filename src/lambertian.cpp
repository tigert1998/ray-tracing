#include <glm/gtc/random.hpp>

#include "lambertian.h"
#include "constant_texture.h"

using boost::optional, boost::none;
using std::pair, std::function, std::shared_ptr, std::make_shared;
using namespace glm;

Lambertian::Lambertian(function<double()> dice, shared_ptr<Texture> texture_ptr) {
    dice_ = dice;
    texture_ptr_ = texture_ptr;
}

Lambertian::Lambertian(std::function<double()> dice, glm::vec3 color) {
	dice_ = dice;
	texture_ptr_ = make_shared<ConstantTexture>(color);
}

optional<pair<vec3, Ray>> Lambertian::Scatter(const Ray &ray, const HitRecord &record) const {
    if (dot(ray.direction(), record.normal) >= 0)
        return none;
    auto p = ray.position() + float(record.t) * ray.direction();
    auto target = p + record.normal + ballRand(0.9f);
    auto albedo = texture_ptr_->Value(0, 0, p);
    return pair<vec3, Ray>(albedo, Ray(p, target - p));
}
