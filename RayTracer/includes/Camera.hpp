#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Ray.hpp"
#include "glm/glm.hpp"

class Camera
{
    public:
        glm::vec3 origin;
        glm::vec3 lower_left_corner;
        glm::vec3 horizontal;
        glm::vec3 vertical;
        Camera()
        {
            lower_left_corner = glm::vec3(-2.0, -1.5, -1.0);
            horizontal = glm::vec3(4.0, 0.0, 0.0);
            vertical = glm::vec3(0.0, 3.0, 0.0);
            origin = glm::vec3(0.0, 0.0, 0.0);
        };
        Ray get_ray(float u, float v)
        {
            return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
        };
};

#endif