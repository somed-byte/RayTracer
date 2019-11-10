#include "AABB.hpp"

bool aabb::hit(const Ray& ray, float t_min, float t_max) const
{
	glm::vec3 invR = 1.0f / ray.direction();
	glm::vec3 tbot = invR * (_min - ray.origin()); // t0x,t0y,t0z
	glm::vec3 ttop = invR * (_max - ray.origin()); // t1x,t1y,t1z

	glm::vec3 tmin = glm::min(tbot, ttop); // minimum x,y,z
	glm::vec3 tmax = glm::max(tbot, ttop); // maximum x,y,z

	// compare x to y/z to get biggest 2 components out of 3
	glm::vec2 xx = tmin.xx;
	glm::vec2 yz = tmin.yz;
	glm::vec2 t = glm::max(xx, yz);
	float t0 = fmax(t.x, t.y);// get the biggest out of 2
	t0 = fmin(t_max, t0);

	// get smallest 2 components out of 3
	xx = tmax.xx;
	yz = tmin.yz;
	t = glm::min(xx, yz);
	float t1 = fmin(t.x, t.y);// get the smallest out of 2
	t1 = fmax(t_min, t1);

	return t0 <= t1;
};

aabb surrounding_box(aabb box0, aabb box1)
{
	glm::vec3 small = glm::min(box0.min(), box1.min());
	glm::vec3 big = glm::max(box0.max(), box1.max());
	return aabb(small, big);
};