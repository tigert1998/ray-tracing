#include "camera.h"

using namespace glm;

Camera::Camera(glm::vec3 position, glm::vec3 look_at, glm::vec3 up, double field_of_view, double width_height_aspect) {
    position_ = position;
    auto w = normalize(position - look_at);
    auto u = normalize(cross(up, w));
    auto v = normalize(cross(w, u));
    double half_height = tan(field_of_view / 2);
    double half_width = width_height_aspect * half_height;
    horizontal_ = u * static_cast<float>(2 * half_width);
    vertical_ = v * static_cast<float>(2 * half_height);
    lower_left_corner_ = position - w - u * static_cast<float>(half_width) - v * static_cast<float>(half_height);
}

Camera::Camera(glm::vec3 position, glm::vec3 lower_left_corner, glm::vec3 horizontal, glm::vec3 vertical) {
    position_ = position;
    lower_left_corner_ = lower_left_corner;
    horizontal_ = horizontal;
    vertical_ = vertical;
}

vec3 Camera::position() const {
    return position_;
}

glm::vec3 Camera::lower_left_corner() const {
    return lower_left_corner_;
}

glm::vec3 Camera::horizontal() const {
    return horizontal_;
}

glm::vec3 Camera::vertical() const {
    return vertical_;
}

Ray Camera::ray_at(double x, double y) const {
    x = (x + 1) / 2;
    y = (y + 1) / 2;
    return Ray(position(), lower_left_corner() + float(x) * horizontal() + float(y) * vertical() - position());
}
