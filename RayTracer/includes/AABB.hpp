#ifndef AABB_HPP
#define AABB_HPP

#include "glm/glm.hpp"
#include "Ray.hpp"

// combine 2 aabb boxes to 1 larger box
// aabb surrounding_box(aabb box0, aabb box1);

class aabb
{
private:
	glm::vec3 _min;
	glm::vec3 _max;
public:
	aabb() {};
	aabb(const glm::vec3& min_point, const glm::vec3& max_point) { _min = min_point; _max = max_point; };
	glm::vec3 min() { return _min; };
	glm::vec3 max() { return _max; };
	bool hit(const Ray& ray, float t_min, float t_max, float& t0, float& t1) const;
};

#endif // !AABB_HPP