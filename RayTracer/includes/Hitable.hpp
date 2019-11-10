#ifndef HITABLE_HPP
#define HITABLE_HPP

#include "Ray.hpp"
#include "AABB.hpp"

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
		virtual bool bounding_box(aabb& box, float t_min, float t_max) const = 0;
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
		virtual bool bounding_box(aabb& box, float t_min, float t_max) const;
};

class HitableList: public Hitable
{
    public:
        Hitable **list;
        int size;
        HitableList() {};
        HitableList(Hitable **l, int n) : list(l), size(n) {};
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
		virtual bool bounding_box(aabb& box, float t_min, float t_max) const;
};

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

#endif