#include "Ray.hpp"

Ray::Ray() {};

Ray::Ray(const glm::vec3 &a, const glm::vec3 &b)
{
    A = a; B = b;
};

glm::vec3 Ray::origin() const
{
    return A;
};

glm::vec3 Ray::direction() const
{
    return B;
};

glm::vec3 Ray::point(float t) const
{
    return A + t * B;
};