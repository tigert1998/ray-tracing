#include "ray.h"

using glm::vec3, glm::normalize;

Ray::Ray(vec3 position, vec3 direction): position_(position) {
    direction_ = normalize(direction);
}

vec3 Ray::position() const {
    return position_;
}

vec3 Ray::direction() const {
    return direction_;
}

