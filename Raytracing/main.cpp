#include <glm/glm.hpp>

#include <chrono>
#include <memory>

#include "Camera.h"
#include "ctmRand.h"
#include "HitableCollection.h"
#include "Material.h"
#include "Ray.h"
#include "Raytracing.h"
#include "Sphere.h"
#include "Window.h"

namespace
{
	std::shared_ptr<HitableCollection> random_scene()
	{
		int n = 500;
		std::vector<std::shared_ptr<Hitable>> list;
		list.push_back(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, std::make_shared<Lambert>(glm::vec3(0.5, 0.5, 0.5))));
		int i = 1;
		for (int a = -11; a < 11; a++)
		{
			for (int b = -11; b < 11; b++)
			{
				float choose_mat = ctmRand();
				glm::vec3 center(a + 0.9 * ctmRand(), 0.2, b + 0.9 * ctmRand());
				if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9)
				{
					if (choose_mat < 0.8)
					{
						list.push_back(std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambert>(glm::vec3(ctmRand() * ctmRand(),
							ctmRand() * ctmRand(),
							ctmRand() * ctmRand()))));
					}
					else if (choose_mat < 0.95)
					{
						list.push_back(std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(glm::vec3(0.5f * (1.0f + ctmRand()),
							0.5f * (1.0f + ctmRand()),
							0.5f * (1.0f + ctmRand())), 0.5f * ctmRand())));
					}
					else
					{
						list.push_back(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dialectric>(1.5)));
					}
				}
			}
		}
		list.push_back(std::make_shared<Sphere>(glm::vec3(0, 1, 0), 1, std::make_shared<Dialectric>(1.5)));
		list.push_back(std::make_shared<Sphere>(glm::vec3(-2, 1, 0), 1, std::make_shared<Lambert>(glm::vec3(0.4, 0.2, 0.1))));
		list.push_back(std::make_shared<Sphere>(glm::vec3(2, 1, 0), 1, std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0)));
		return std::make_shared<HitableCollection>(list);
	}
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
	uint32_t nx = 600;
	uint32_t ny = 300;
	uint8_t ns = 1;

	glm::vec3 lookFrom(3, 1.5, 3);
	glm::vec3 lookAt(0, 0, -1);
	float dist_to_focus = static_cast<float>((lookFrom - lookAt).length());
	std::shared_ptr<Camera> cam = std::make_shared<Camera>(lookFrom, lookAt, glm::vec3(0, 1, 0), 20, static_cast<float>(nx) / ny, 2, dist_to_focus);

	Raytracing tracing(nx, ny, ns);
	tracing.setCamera(cam);
	tracing.setCollection(random_scene());

	Window win(instance, nx, ny);
	tracing.process(win); // fill the background of the window with the default color when starting
	tracing.start();
	win.render();

	float elapsed = 0;
	auto startTick = std::chrono::steady_clock::now();
	while (win.isOpen())
	{
		if (elapsed > 1000.0f / 12)
		{
			if (tracing.isRunning())
				tracing.process(win);

			win.handleEvents();
			win.render();
			elapsed = 0;
		}

		auto endTick = std::chrono::steady_clock::now();
		elapsed += static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(endTick - startTick).count());
		startTick = endTick;
	}
	
	return (0);
}