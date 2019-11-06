#include "Hitable.hpp"

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
	// Equation: dot((Ray(t) - center),(Ray(t) - center)) = R^2
	// dot((origin + t * dir - center),(origin + t * dir - center)) = R^2
	// final formula: t^2 * dot(dir,dir) + 2 * t * dot(dir, origin - center) + ||origin - center|| - R^2 =0

	// discriminant > 0 -> intersection
    glm::vec3 oc = r.origin() - center;
    float a = glm::dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0)
    {
		// 
        float solution = (-b - sqrt(discriminant))/a;
        if (solution < t_max && solution > t_min)
        {
            rec.t = solution; // solution t
            rec.p = r.point(rec.t); // origin + t * dir
            rec.normal = (rec.p - center) / radius; // normalized
            rec.mat_ptr = mat;
            return true;
        }
        solution = (-b + sqrt(discriminant))/a;
        if (solution < t_max && solution > t_min)
        {
            rec.t = solution;
            rec.p = r.point(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }
    }
    return false;
};

bool HitableList::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
    hit_record tmp_rec;
    bool hit_anything = false;
	// hit every object to update the t_max(process of reducing)
    double closest_so_far = t_max;
    for (int i = 0; i < size; ++i)
    {
        if (list[i]->hit(r, t_min, closest_so_far, tmp_rec))
        {
            hit_anything = true;
            closest_so_far = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_anything;
};