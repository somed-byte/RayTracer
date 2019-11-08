#include "Camera.hpp"

glm::vec3 random_in_unit_disk()
{
	glm::vec3 p;
	do
	{
		p = 2.0f * glm::vec3(wdrand48(), wdrand48(), 0) - glm::vec3(1, 1, 0);
	} while (glm::length(p) >= 1.0f);
	return p;
};

glm::vec3 random_in_unit_disk_no_loop()
{
	float angle = wdrand48() * 2 * M_PI;
	float length = wdrand48();
	glm::vec3 p = length * glm::vec3(cos(angle), sin(angle), 0);
	return p;
};

/*
Camera::Camera(float fov, float aspect, glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
    float theta = fov * M_PI/180;
    float h_height = tan(theta/2);
    float h_width = aspect * h_height;

    origin = pos;
    
    glm::vec3 front = glm::normalize(pos - target);
    glm::vec3 right = glm::normalize(glm::cross(up, front));
    glm::vec3 cam_up = glm::cross(front, right);

    lower_left_corner = origin - h_width*right -h_height*cam_up - front;
    horizontal = 2 * h_width * right;
    vertical = 2 * h_height * cam_up;
};
*/

Camera::Camera(float fov, float aspect, glm::vec3 pos, glm::vec3 target, glm::vec3 up, float aperture, float focusDis)
{
	lensRadius = aperture / 2.0f;
	float theta = fov * M_PI / 180;
	float h_height = tan(theta / 2);
	float h_width = aspect * h_height;

	origin = pos;
	// positive z axis is point out of screen, so front = pos - target, or the camera will flip
	glm::vec3 front = glm::normalize(pos - target);
	glm::vec3 right = glm::normalize(glm::cross(up, front));
	glm::vec3 cam_up = glm::cross(front, right);

	lower_left_corner = origin - h_width * right * focusDis - h_height * cam_up * focusDis - front * focusDis;
	horizontal = 2 * h_width * right * focusDis;
	vertical = 2 * h_height * cam_up * focusDis;
};

Ray Camera::get_ray(float u, float v)
{
	glm::vec3 offset = lensRadius * random_in_unit_disk_no_loop();
    return Ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset);
};