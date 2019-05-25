#include <fstream>
#include <iostream>
#include <random>

//#include <math.h>
#include <ctime>

#include <glm/glm.hpp>

#include "Camera.h"
#include "HitableCollection.h"
#include "Ray.h"
#include "Sphere.h"
#include "Material.h"

HitableCollection *random_scene() {
	int n = 500;
	std::vector<std::shared_ptr<Hitable>> list;
	list.push_back(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, new Lambert(glm::vec3(0.5, 0.5, 0.5))));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = static_cast<float>(std::rand() % 100) / 100;
			glm::vec3 center(a + 0.9 * static_cast<float>(std::rand() % 100) / 100, 0.2, b + 0.9 * static_cast<float>(std::rand() % 100) / 100);
			if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list.push_back(std::make_shared<Sphere>(center, 0.2, new Lambert(glm::vec3(static_cast<float>(std::rand() % 100) / 100 * static_cast<float>(std::rand() % 100) / 100,
						static_cast<float>(std::rand() % 100) / 100 * static_cast<float>(std::rand() % 100) / 100,
						static_cast<float>(std::rand() % 100) / 100 * static_cast<float>(std::rand() % 100) / 100))));
				}
				else if (choose_mat < 0.95) { // metal
					list.push_back(std::make_shared<Sphere>(center, 0.2, new Metal(glm::vec3(0.5f * (1.0f + static_cast<float>(std::rand() % 100) / 100),
						0.5f * (1.0f + static_cast<float>(std::rand() % 100) / 100),
							0.5f * (1.0f + static_cast<float>(std::rand() % 100) / 100)), 0.5f * static_cast<float>(std::rand() % 100) / 100)));
				}
				else {  // glass
					list.push_back(std::make_shared<Sphere>(center, 0.2, new Dialectric(1.5)));
				}
			}
		}
	}

	list.push_back(std::make_shared<Sphere>(glm::vec3(0, 1, 0), 1, new Dialectric(1.5)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(-4, 1, 0), 1, new Lambert(glm::vec3(0.4, 0.2, 0.1))));
	list.push_back(std::make_shared<Sphere>(glm::vec3(4, 1, 0), 1, new Metal(glm::vec3(0.7, 0.6, 0.5), 0)));

	return new HitableCollection(list);
}

glm::vec3 color(const Ray& ray, const Hitable* world, int depth)
{
	HitRecord record;
	if (world->hit(ray, 0.001, 100, record))
	{
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 50 && record.material->scatter(ray, record, attenuation, scattered))
		{
			return (attenuation * color(scattered, world, depth + 1));
		}
		else
			return (glm::vec3(0, 0, 0));
	}
	else
	{
		glm::vec3 direction = glm::normalize(ray.direction());
		float t = 0.5 * (direction.y + 1);
		return ((1 - t) * glm::vec3(1, 1, 1) + t * glm::vec3(0.5, 0.7, 1));
	}
}

int main()
{
	int nx = 1280;
	int ny = 720;
	int ns = 100;

	glm::vec3 lookFrom(3, 3, 1);
	glm::vec3 lookAt(0, 1, 1);
	float dist_to_focus = (lookFrom - lookAt).length();	
	float aperture = 2;
	Camera cam(lookFrom, lookAt, glm::vec3(0, 1, 0), 20, nx / ny, aperture, dist_to_focus);

	HitableCollection *collection = random_scene();

	std::srand(std::time(nullptr));
	std::ofstream file("output.ppm");
	file << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; --j)
	{
		for (int i = 0; i < nx; ++i)
		{
			glm::vec3 rgb(0, 0, 0);
			for (int s = 0; s < ns; ++s)
			{
				float u = (static_cast<float>(i) + (static_cast<float>(std::rand() % 100) / 100)) / static_cast<float>(nx);
				float v = (static_cast<float>(j) + (static_cast<float>(std::rand() % 100) / 100)) / static_cast<float>(ny);
				Ray ray = cam.getRay(u, v);
				rgb += color(ray, collection, 0);
			}
			rgb /= static_cast<float>(ns);
			rgb = glm::vec3(sqrt(rgb[0]), sqrt(rgb[1]), sqrt(rgb[2]));

			int ir = static_cast<int>(255.99 * rgb[0]);
			int ig = static_cast<int>(255.99 * rgb[1]);
			int ib = static_cast<int>(255.99 * rgb[2]);
			file << ir << " " << ig << " " <<ib << "\n";
		}

		std::cout << "\r[";
		for (int idx = 0; idx < 100; ++idx)
		{
			if (idx < static_cast<int>((1 - static_cast<float>(j * nx) / static_cast<float>(ny * nx)) * 100))
				std::cout << "|";
			else
				std::cout << "-";
		}
		std::cout << "] " << static_cast<int>((1 - static_cast<float>(j * nx) / static_cast<float>(ny * nx)) * 100) << "%" << std::flush;
	}
	file.close();
	return (0);
}