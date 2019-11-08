#include "Camera.hpp"

Camera::Camera(float fov, float aspect, glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
    float theta = fov * M_PI/180;
    float h_height = tan(theta/2);
    float h_width = aspect * h_height;

    origin = pos;
    // positive z axis is point out of screen, so front = pos - target, or the camera will flip
    glm::vec3 front = glm::normalize(pos - target);
    glm::vec3 right = glm::normalize(glm::cross(up, front));
    glm::vec3 cam_up = glm::cross(front, right);

    lower_left_corner = origin - h_width*right -h_height*cam_up - front;
    horizontal = 2 * h_width * right;
    vertical = 2 * h_height * cam_up;
};

Ray Camera::get_ray(float u, float v)
{
    return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
};