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

bool Rect::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
	// TODO
	float tnear, tfar;
	if (xybox.hit(r, t_min, t_max, tnear, tfar))
	{
		rec.t = tnear;
		rec.p = r.point(tnear);
		rec.normal = normal;
		rec.mat_ptr = mat;
		return true;
	}
	return false;
};

Cube::Cube(glm::vec3 center, glm::vec3 len, glm::vec3 _rotation, material *mat)
{
	rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, _rotation.x, glm::vec3(1, 0, 0));
	rotation = glm::rotate(rotation, _rotation.y, glm::vec3(0, 1, 0));
	rotation = glm::rotate(rotation, _rotation.z, glm::vec3(0, 0, 1));

	glm::vec4 c1 = glm::vec4(center.x + len.x, center.y - len.y, center.z - len.z, 1.0);
	glm::vec4 c2 = glm::vec4(center.x - len.x, center.y - len.y, center.z - len.z, 1.0);
	glm::vec4 c3 = glm::vec4(center.x - len.x, center.y - len.y, center.z + len.z, 1.0);
	glm::vec4 c4 = glm::vec4(center.x + len.x, center.y - len.y, center.z + len.z, 1.0);

	glm::vec4 c5 = glm::vec4(center.x + len.x, center.y + len.y, center.z - len.z, 1.0);
	glm::vec4 c6 = glm::vec4(center.x - len.x, center.y + len.y, center.z - len.z, 1.0);
	glm::vec4 c7 = glm::vec4(center.x - len.x, center.y + len.y, center.z + len.z, 1.0);
	glm::vec4 c8 = glm::vec4(center.x + len.x, center.y + len.y, center.z + len.z, 1.0);

	c1 = rotation * c1;
	c2 = rotation * c2;
	c3 = rotation * c3;
	c4 = rotation * c4;
	c5 = rotation * c5;
	c6 = rotation * c6;
	c7 = rotation * c7;
	c8 = rotation * c8;

	glm::vec3 n1 = rotation * glm::vec4(0, 0, -1, 0);
	glm::vec3 n2 = rotation * glm::vec4(0, 0, 1, 0);
	glm::vec3 n3 = rotation * glm::vec4(-1, 0, 0, 0);
	glm::vec3 n4 = rotation * glm::vec4(1, 0, 0, 0);
	glm::vec3 n5 = rotation * glm::vec4(0, -1, 0, 0);
	glm::vec3 n6 = rotation * glm::vec4(0, 1, 0, 0);

	Hitable** l = new Hitable*[6];
	int i = 0;
	l[i++] = new Rect(c2.x, c1.x, c2.y, c5.y, c1.z, c1.z + 0.001f, n1, mat);
	l[i++] = new Rect(c2.x, c1.x, c2.y, c5.y, c3.z, c3.z + 0.001f, n2, mat);

	l[i++] = new Rect(c2.x, c2.x + 0.001f, c2.y, c5.y, c1.z, c3.z, n3, mat);
	l[i++] = new Rect(c1.x, c1.x + 0.001f, c2.y, c5.y, c1.z, c3.z, n4, mat);

	l[i++] = new Rect(c2.x, c1.x, c2.y, c2.y + 0.001f, c1.z, c3.z, n5, mat);
	l[i++] = new Rect(c2.x, c1.x, c5.y, c5.y + 0.001f, c1.z, c3.z, n5, mat);
	/*
	l[i++] = new Rect(center.x - len.x, center.x + len.x, center.y - len.y, center.y + len.y, center.z - len.z, center.z - len.z + 0.001f, glm::vec3(0, 0, -1), mat);
	l[i++] = new Rect(center.x - len.x, center.x + len.x, center.y - len.y, center.y + len.y, center.z + len.z, center.z + len.z + 0.001f, glm::vec3(0, 0, 1), mat);

	l[i++] = new Rect(center.x - len.x, center.x - len.x + 0.001f, center.y - len.y, center.y + len.y, center.z - len.z, center.z + len.z, glm::vec3(-1, 0, 0), mat);
	l[i++] = new Rect(center.x + len.x, center.x + len.x + 0.001f, center.y - len.y, center.y + len.y, center.z - len.z, center.z + len.z, glm::vec3(1, 0, 0), mat);

	l[i++] = new Rect(center.x - len.x, center.x + len.x, center.y - len.y, center.y - len.y + 0.001f, center.z - len.z, center.z + len.z, glm::vec3(0, -1, 0), mat);
	l[i++] = new Rect(center.x - len.x, center.x + len.x, center.y + len.y, center.y + len.y + 0.001f, center.z - len.z, center.z + len.z, glm::vec3(0, 1, 0), mat);
	*/
	list = new HitableList(l, i);
};

bool Cube::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const
{
	return list->hit(r, t_min, t_max, rec);
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