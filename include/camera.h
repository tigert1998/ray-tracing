#pragma once

#include <boost/optional.hpp>

#include <glm/glm.hpp>

#include "ray.h"

class Camera {
private:
    glm::vec3 position_, lower_left_corner_, horizontal_, vertical_, u_, v_, w_;
    bool is_defocus_camera_;
    boost::optional<double> lens_radius_;

public:
    Camera() = delete;
    Camera(glm::vec3 position, glm::vec3 look_at, glm::vec3 up, double field_of_view, double width_height_aspect);
    Camera(glm::vec3 position, glm::vec3 look_at, glm::vec3 up, double field_of_view, double width_height_aspect, double aperture, double focus_distance);
    Camera(glm::vec3 position, glm::vec3 lower_left_corner, glm::vec3 horizontal, glm::vec3 vertical);
    glm::vec3 position() const;
    glm::vec3 lower_left_corner() const;
    glm::vec3 horizontal() const;
    glm::vec3 vertical() const;
    Ray ray_at(double x, double y) const;
    bool is_defocus_camera() const;
};
