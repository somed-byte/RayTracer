#include "AABB.hpp"

bool aabb::hit(const Ray& ray, float t_min, float t_max, float& t0, float& t1) const
{
	glm::vec3 invR = 1.0f / ray.direction();
	glm::vec3 tbot = invR * (_min - ray.origin()); // t0x,t0y,t0z
	glm::vec3 ttop = invR * (_max - ray.origin()); // t1x,t1y,t1z

	glm::vec3 tmin = glm::min(tbot, ttop); // minimum x,y,z
	glm::vec3 tmax = glm::max(tbot, ttop); // maximum x,y,z

	// compare x to y/z to get biggest 2 components out of 3
	glm::vec2 xx = glm::vec2(tmin.x, tmin.x);
	glm::vec2 yz = glm::vec2(tmin.y, tmin.z);
	glm::vec2 t = glm::max(xx, yz);
	t0 = fmax(t.x, t.y);// get the biggest out of 2
	t0 = t0 > t_min ? t0 : t_min;

	// get smallest 2 components out of 3
	xx = glm::vec2(tmax.x, tmax.x);
	yz = glm::vec2(tmax.y, tmax.z);
	t = glm::min(xx, yz);
	t1 = fmin(t.x, t.y);// get the smallest out of 2
	t1 = t1 < t_max ? t1 : t_max;

	return t0 <= t1;
};

/* //TODO
aabb surrounding_box(aabb box0, aabb box1)
{
	glm::vec3 small = glm::min(box0.min(), box1.min());
	glm::vec3 big = glm::max(box0.max(), box1.max());
	return aabb(small, big);
};
*/