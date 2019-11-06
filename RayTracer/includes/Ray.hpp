#ifndef RAY_HPP
#define RAY_HPP

#include "glm/glm.hpp"

class Ray
{
    private:
        glm::vec3 A;//start point
        glm::vec3 B;//towards direction

    public:
        Ray();
        Ray(const glm::vec3 &a, const glm::vec3 &b);
        glm::vec3 origin() const;
        glm::vec3 direction() const;
        glm::vec3 point(float t) const;
};

#endif