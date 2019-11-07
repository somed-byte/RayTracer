#include "material.hpp"

glm::vec3 random_in_unit_sphere()
{
	glm::vec3 p;
	do {
		// random point in unit cube
		// cast range to [0,2] then minus (1,1,1) cast range to [-1,1]
		p = 2.0f * glm::vec3(wdrand48(), wdrand48(), wdrand48()) - glm::vec3(1, 1, 1);
	} while (glm::length(p) >= 1.0f);// reject the point if it is outside the sphere

	return p;
}

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
{
	return v - 2 * glm::dot(v, n) * n;
}

bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted)
{
	glm::vec3 view = glm::normalize(v);
	float dt = glm::dot(view, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (view - n * dt) - n * (float)sqrt(discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}