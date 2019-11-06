#ifndef HITABLE_HPP
#define HITABLE_HPP

#include "Ray.hpp"

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
};

class HitableList: public Hitable
{
    public:
        Hitable **list;
        int size;
        HitableList() {};
        HitableList(Hitable **l, int n) : list(l), size(n) {};
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
};

#endif