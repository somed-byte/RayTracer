#ifndef CAMERA_HPP
#define CAMERA_HPP

#define _USE_MATH_DEFINES
#include "Ray.hpp"
#include "glm/glm.hpp"
#include "drand48.hpp"
#include <math.h>

glm::vec3 random_in_unit_disk();
glm::vec3 random_in_unit_disk_no_loop();

class Camera
{
    public:
        glm::vec3 origin;
        glm::vec3 lower_left_corner;
        glm::vec3 horizontal; // vec3(width, 0, 0)
        glm::vec3 vertical; // vec3(0, height, 0)
		float lensRadius;
        
		/*
		Camera()
        {
            lower_left_corner = glm::vec3(-2.0, -1.5, -1.0);
            horizontal = glm::vec3(4.0, 0.0, 0.0);
            vertical = glm::vec3(0.0, 3.0, 0.0);
            origin = glm::vec3(0.0, 0.0, 0.0);
        };
		*/
        // Camera(float fov, float aspect, glm::vec3 pos, glm::vec3 target, glm::vec3 up);
		Camera(float fov, float aspect, glm::vec3 pos, glm::vec3 target, glm::vec3 up, float aperture, float focusDis);
        Ray get_ray(float u, float v);
};

#endif