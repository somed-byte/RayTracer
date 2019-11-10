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

bool Rect_xy::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
	/* // TODO
	float tnear, tfar;
	if (xybox.hit(r, t_min, t_max, tnear, tfar))
	{
		rec.t = tnear;
		rec.p = r.point(tnear);
		rec.normal = glm::vec3(0, 0, 1);
		rec.mat_ptr = mat;
		return true;
	}
	return false;
	*/
	float t = (z-r.origin().z )/ r.direction().z;
    if (t < t_min || t > t_max)
        return false;
    float x = r.origin().x + t*r.direction().x;
    float y = r.origin().y + t*r.direction().y;
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.t = t;
    rec.mat_ptr = mat;
    rec.p = r.point(t);
    rec.normal = glm::vec3(0, 0, 1);
    return true;
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

/* //TODO
bool HitableList::bounding_box(aabb& box, float t_min, float t_max) const
{
	if (size < 1) return false;
	aabb tmp_box;
	bool result = list[0]->bounding_box(tmp_box, t_min, t_max);
	if (!result)
	{
		return false;
	}
	else
	{
		box = tmp_box;
	}
	for (int i = 1; i < size; ++i)
	{
		if (list[i]->bounding_box(tmp_box, t_min, t_max))
		{
			box = surrounding_box(box, tmp_box);// combine the bounding boxes
		}
		else
		{
			return false;
		}
	}
	return true;
};

// this function dont need t_min & t_max
bool Sphere::bounding_box(aabb& box, float t_min, float t_max) const
{
};

int bvh_x_compare (const void *a, const void *b)
{
	aabb leftbox, rightbox;
	Hitable *aa = *(Hitable**)a;
	Hitable *bb = *(Hitable**)b;

	if (!aa->bounding_box(leftbox, 0, 0) || !bb->bounding_box(rightbox, 0, 0))
	{
		std::cerr << "no bounding box in bvh node constructor\n";
	}

	if (leftbox.min().x < rightbox.min().x)
	{
		return -1;
	}
	else
	{
		return 1;
	}
};

bvh_node::bvh_node(Hitable** l, int n)
{
	qsort(l, n, sizeof(Hitable *), bvh_x_compare);

	if(n==1)
	{
		left = right = l[0];
	}
	else if(n==2)
	{
		left = l[0];
		right = l[1];
	}
	else
	{
		left = new bvh_node(l, n/2);
		right = new bvh_node(l + n/2, n - n/2); // change first pointer to l + n/2 , size reduce n/2
	}
	
	aabb leftbox, rightbox;
	if (!left->bounding_box(leftbox, 0, 0) || !right->bounding_box(rightbox, 0, 0))
	{
		std::cerr << "no bounding box in bvh node constructor\n";
	}

	bvh_box = surrounding_box(leftbox, rightbox);
};

bool bvh_node::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
	if (bvh_box.hit(r, t_min, t_max))
	{
		hit_record lrec, rrec;
		bool lhit = left->hit(r, t_min, t_max, lrec);
		bool rhit = right->hit(r, t_min, t_max, rrec);
		if (lhit && rhit)
		{
			rec = lrec.t < rrec.t ? lrec : rrec;
			return true;
		}
		else if (lhit)
		{
			rec = lrec;
			return true;
		}
		else if (rhit)
		{
			rec = rrec;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
};

bool bvh_node::bounding_box(aabb& box, float t_min, float t_max) const {
	box = bvh_box;
	return true;
}
*/