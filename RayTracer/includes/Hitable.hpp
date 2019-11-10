#ifndef HITABLE_HPP
#define HITABLE_HPP

#include "Ray.hpp"
#include "AABB.hpp"
#include <iostream>

class material;

struct hit_record
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    material *mat_ptr;
};

class Hitable
{
    public:
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
		//virtual bool bounding_box(aabb& box, float t_min, float t_max) const = 0;
};

class Sphere: public Hitable
{   
    public:
        glm::vec3 center;
        float radius;
        material *mat;
        Sphere() {};
        Sphere(glm::vec3 _center, float r) : center(_center), radius(r) {};
        Sphere(glm::vec3 _center, float r, material* _mat) : center(_center), radius(r), mat(_mat) {};
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
		//virtual bool bounding_box(aabb& box, float t_min, float t_max) const;
};

class Rect_xy: public Hitable
{
    public:
        float x0, x1, y0, y1, z;
        material *mat;
        aabb xybox;
        Rect_xy() {};
        Rect_xy(float _x0, float _x1, float _y0, float _y1, float _z, material* _mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), z(_z),mat(_mat)
        {
            // xybox = aabb(glm::vec3(x0, y0, z - 0.0001), glm::vec3(x1, y1, z + 0.0001));
        };
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
        //virtual bool bounding_box(aabb& box, float t_min, float t_max) const;
};

class HitableList: public Hitable
{
    public:
        Hitable **list;
        int size;
        HitableList() {};
        HitableList(Hitable **l, int n) : list(l), size(n) {};
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
		//virtual bool bounding_box(aabb& box, float t_min, float t_max) const;
};

/* TODO
class bvh_node : public Hitable
{
	aabb bvh_box;
	Hitable* left;
	Hitable* right;
public:
	bvh_node() {};
	bvh_node(Hitable** l, int n);
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(aabb& box, float t_min, float t_max) const;
};

int bvh_x_compare (const void *a, const void *b);
*/
#endif