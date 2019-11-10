#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "drand48.hpp"
#include "Hitable.hpp"
#include "Ray.hpp"
#include "glm/glm.hpp"

struct hit_record;

glm::vec3 random_in_unit_sphere();

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n);

bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted);

float schlick(float cosine, float ref_idx);

class material
{
    public:
        virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
        virtual glm::vec3 emitted() const
        {
            // for those which are not light source just return black and do need to implement this function
            return glm::vec3(0.0, 0.0, 0.0);
        };
};

class lambertian : public material
{
    public:
        glm::vec3 albedo;
        lambertian(const glm::vec3& a): albedo(a) {}
        virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered) const
        {
            glm::vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = Ray(rec.p, target - rec.p);
            attenuation = albedo;
            return true;
        };
};

class metal : public material
{
    public:
        glm::vec3 albedo;
        // fuzz controls the radius of random unit sphere
        // the bigger the sphere, the fuzzier the reflection will be
        float fuzz;
        metal(const glm::vec3& a, float _f) : albedo(a), fuzz(_f) {};
        virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered) const
        {
            glm::vec3 reflected = reflect(glm::normalize(r_in.direction()), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return (glm::dot(scattered.direction(), rec.normal) > 0);
        }
};

class dielectirc : public material
{
    public:
        glm::vec3 albedo;
        float ref_idx;
        dielectirc(const glm::vec3& a, float _idx) : albedo(a), ref_idx(_idx) {};
        virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered) const
        {
            attenuation = albedo;

            glm::vec3 outward_normal;
            glm::vec3 reflected = reflect((r_in.direction()), rec.normal);
            glm::vec3 refracted;

            float ni_over_nt;
            float reflect_prob;
            float cosine;

            // Process Ray Enter/Exit
            if (glm::dot(r_in.direction(), rec.normal) > 0) // ray shoot through object into vacuum
            {
                outward_normal = -rec.normal;
                ni_over_nt = ref_idx;
                cosine = glm::dot(glm::normalize(r_in.direction()), rec.normal);
            }
            else // ray shoots into object
            {
                outward_normal = rec.normal;
                ni_over_nt = 1.0 / ref_idx;
                cosine = -glm::dot(glm::normalize(r_in.direction()), rec.normal);
            }
            
            // Process Ray Reflection/Refraction
            if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
            {
                reflect_prob = schlick(cosine, ref_idx);
            }
            else
            {
                reflect_prob = 1.0;
            }
            // use drand48() to decide reflection or refraction
            if (wdrand48() < reflect_prob)
            {
                scattered = Ray(rec.p, reflected);
            } 
            else
            {
                scattered = Ray(rec.p, refracted);
            }

            return true;
        }
};

class DiffuseLight : public material
{
    glm::vec3 color;
    public:
        DiffuseLight(glm::vec3 light_col) : color(light_col) {};
        virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered) const
        {
            return false;
        };
        virtual glm::vec3 emitted() const
        {
            return color;
        };
};

#endif