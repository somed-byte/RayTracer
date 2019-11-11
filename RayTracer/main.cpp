#include <iostream>
#include <fstream>
#include <float.h>
#include "Camera.hpp"
#include "Ray.hpp"
#include "Hitable.hpp"
#include "material.hpp"

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

const int nx = 800;
const int ny = 600;
const int ns = 200;

void printProgress(double percentage);
void WritePPM_P3(const char* filename, int nx, int ny);
void WritePPM_P6(const char* filename, int nx, int ny);
glm::vec3 color(const Ray& r, Hitable *world, int depth);

Hitable* simple_scene()
{
	Hitable **list = new Hitable*[7];
	list[0] = new Sphere(glm::vec3(0, 0, -1.5), 0.5, new lambertian(glm::vec3(0.3, 0.3, 0.8)));
	list[1] = new Sphere(glm::vec3(0, -100.5, -1.5), 100, new lambertian(glm::vec3(0.5, 0.5, 0.5)));
	list[2] = new Sphere(glm::vec3(1.2, 0, -1.5), 0.5, new lambertian(glm::vec3(0.8, 0.6, 0.2)));
	list[3] = new Sphere(glm::vec3(-1.8, 0, -1.5), 0.5, new metal(glm::vec3(0.3, 0.9, 0.3), 0.01));
	list[4] = new Sphere(glm::vec3(0.4, -0.2, -1.0), 0.3, new dielectirc(glm::vec3(1.0, 1.0, 1.0), 1.3));
	list[5] = new Sphere(glm::vec3(-0.5, -0.15, -1.0), 0.35, new dielectirc(glm::vec3(1.0, 0, 0), 1.1));
	list[6] = new Sphere(glm::vec3(-0.5, -0.15, -1.0), -0.3, new dielectirc(glm::vec3(1.0, 0, 0), 1.1));
	return new HitableList(list, 7);
}

Hitable* light_scene()
{
	Hitable** list = new Hitable * [5];
	list[0] = new Sphere(glm::vec3(0, 0, -1.5), 0.5, new lambertian(glm::vec3(0.3, 0.3, 0.8)));
	list[1] = new Sphere(glm::vec3(0, -1000.5, -1.5), 1000, new lambertian(glm::vec3(0.5, 0.5, 0.5)));
	list[2] = new Sphere(glm::vec3(2.0, 0, -1.5), 0.5, new lambertian(glm::vec3(0.8, 0.6, 0.2)));
	list[3] = new Sphere(glm::vec3(1.0, 0, -1.0), 0.3, new DiffuseLight(glm::vec3(4.0, 4.0, 4.0)));
	list[4] = new Rect(-1, 1, 0, 1, -5, -5 + 0.0001, glm::vec3(0, 0, 1), new DiffuseLight(glm::vec3(4.0, 4.0, 4.0)));
	return new HitableList(list, 5);
}

Hitable* cornell_box()
{
	Hitable** list = new Hitable * [9];
	int i = 0;
	material* red = new lambertian(glm::vec3(0.65, 0.05, 0.05));
	material* green = new lambertian(glm::vec3(0.12, 0.45, 0.15));
	material* white = new lambertian(glm::vec3(0.73, 0.73, 0.73));
	material* light = new DiffuseLight(glm::vec3(15, 15, 15));
	// cornell box: 5
	//x0  x1  y0  y1  z0  z1
	list[i++] = new Rect(5, 5.001f, 0, 5, 0, 5, glm::vec3(-1, 0, 0), green);
	list[i++] = new Rect(0, 0.001f, 0, 5, 0, 5, glm::vec3(1, 0, 0), red);
	list[i++] = new Rect(0, 5, 5, 5.001f, 0, 5, glm::vec3(0, -1, 0), white);
	list[i++] = new Rect(0, 5, 0, 0.001f, 0, 5, glm::vec3(0, 1, 0), white);
	list[i++] = new Rect(0, 5, 0, 5, 5, 5.001f, glm::vec3(0, 0, -1), white);
	// diffuse light: 1
	list[i++] = new Rect(2.2, 3.4, 5, 5.001f, 2.3, 3.3, glm::vec3(0, -1, 0), light);

	// inside the box
		// mirrors
	list[i++] = new Rect(4.99, 4.991f, 1.4, 4.1, 1.1, 4.4, glm::vec3(-1, 0, 0), new metal(glm::vec3(1.0, 1.0, 1.0), 0.0));
	list[i++] = new Sphere(glm::vec3(1.5, 1, 1.), 0.8, white);
	list[i++] = new Cube(glm::vec3(3.0, 1, 2.5), glm::vec3(0.6, 1, 0.6), glm::vec3(0, glm::radians(45.0f), 0), white);

	return new HitableList(list, i);
}

Camera set_camera_simple()
{
	float FOV = 60.0;
	glm::vec3 cam_pos(2, 2, 1);
	glm::vec3 cam_target(0, 0, 0);
	float cam_aperture = 0.05;
	float focus_dis = glm::length(cam_pos - cam_target);
	Camera cam(FOV, float(nx) / float(ny), cam_pos, cam_target, glm::vec3(0.0, 1.0, 0.0), cam_aperture, focus_dis);
	return cam;
}

Camera set_camera_cornell_box()
{
	float FOV = 40.0;
	glm::vec3 cam_pos(2.5, 2.5, -7);
	glm::vec3 cam_target(2.5, 2.5, 0);
	float cam_aperture = 0.0;
	float focus_dis = 10;
	Camera cam(FOV, float(nx) / float(ny), cam_pos, cam_target, glm::vec3(0.0, 1.0, 0.0), cam_aperture, focus_dis);
	return cam;
}

int main(void)
{
    // P6_Intersect("./outputs/pure_color_sphere.ppm", 512, 512);
	int progress_percentage = 0;
    
	Hitable* world = cornell_box();
	Camera cam = set_camera_cornell_box();

    std::ofstream output;
#ifdef __APPLE__
    output.open("./outputs/MacOS_cornell_box_final.ppm", std::ofstream::binary);
#elif defined(_WIN32) || defined(_WIN64)
    output.open("D:\\C++Projects\\RayTracer\\outputs\\Win32_cornell_box_with_mirror.ppm", std::ofstream::binary);
#endif
    output << "P6\n" << nx << "\n" << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
			progress_percentage++;
			// final color 4 each pixel
			glm::vec3 c = glm::vec3(0.0f, 0.0f, 0.0f);
            // Antialiasing
            for(int s = 0; s < ns; ++s)
            {
                float u = float(i + wdrand48())/float(nx);
                float v = float(j + wdrand48())/float(ny);
                Ray r = cam.get_ray(u, v);
                c += color(r, world, 0);
            }
            c /= float(ns);
            //gamma correction
            c = glm::sqrt(c);
            
            unsigned char ri = (unsigned char)(255.99f * c.r);
            unsigned char gi = (unsigned char)(255.99f * c.g);
            unsigned char bi = (unsigned char)(255.99f * c.b);
            output.write((char*) &ri, sizeof(ri));
            output.write((char*) &gi, sizeof(gi));
            output.write((char*) &bi, sizeof(bi));
			printProgress(progress_percentage/(double)(nx * ny));
        }
    }
    output.close();

    delete world;
}

/// @para: depth(each ray max-hits) 
glm::vec3 color(const Ray& r, Hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.0001, FLT_MAX, rec))
    {
        Ray scattered;
        glm::vec3 attenuation; // to store the current hit color
        glm::vec3 emitted = rec.mat_ptr->emitted();
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted + attenuation * color(scattered, world, depth+1);
        }
        else
        {
            return emitted;
        }
    }
    else
    {
        // ! DEPRECATED! dont use gradient color for background anymore
        // for background color generation
        // return glm::vec3(1.0, 1.0, 1.0);
        ////glm::vec3 n_direction = glm::normalize(r.direction());
        ////float t = 0.5 * n_direction.y + 0.5;
        ////return glm::vec3(1.0, 1.0, 1.0) * (1-t) + t * glm::vec3(0.5, 0.8, 1.0);
        return glm::vec3(0,0,0);
    }
}

// P3 formate is storaged in ASCII, which make it larger but readable
void WritePPM_P3(const char* filename, int nx, int ny)
{
    std::ofstream output;
    output.open(filename);
    output << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            float r = float(i)/float(nx);
            float g = float(j)/float(ny);
            float b = 0.2;
            int ri = int(255.99f * r);
            int gi = int(255.99f * g);
            int bi = int(255.99f * b);
            output << ri << " " << gi << " " << bi;
            if (i != nx -1) output << " ";// Add space between each color but removed at the end of the line
        }
        output << "\n";
    }
    output.close();
}

// P6 format will make file smaller because it is storaged in binary
// don't need space between each color and each channel
// each pixel occupy 24 bits , 8 bits for each channel
void WritePPM_P6(const char* filename, int nx, int ny)
{
    std::ofstream output;
    output.open(filename);
    output << "P6\n" << nx << "\n" << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            float r = float(i)/float(nx);
            float g = float(j)/float(ny);
            float b = 0.2;
            unsigned char ri = (unsigned char)(255.99f * r);
            unsigned char gi = (unsigned char)(255.99f * g);
            unsigned char bi = (unsigned char)(255.99f * b);
            output.write((char*) &ri, sizeof(ri));
            output.write((char*) &gi, sizeof(gi));
            output.write((char*) &bi, sizeof(bi));
        }
    }
    output.close();
}

void printProgress(double percentage)
{
	int val = (int)(percentage * 100);
	int lpad = (int)(percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
	fflush(stdout);
}