#pragma once

#include <glm/glm.hpp>

#include <array>
#include <chrono>
#include <vector>

#include "Section.h"

class Camera;
class HitableCollection;
class Window;

typedef std::chrono::time_point<std::chrono::steady_clock> Clock;

class Raytracing
{
	bool								_isRunning;

	uint32_t							_nx;
	uint32_t							_ny;
	uint8_t								_ns;

	std::vector<glm::vec3>				_pic;

	std::shared_ptr<Camera>				_cam;
	std::shared_ptr<HitableCollection>	_collection;

	uint8_t								_activeSection;
	std::array<Section, 4>				_sections;

	Clock								_startTime;

public:
	Raytracing(uint32_t width, uint32_t height, uint8_t nbSamples);
	~Raytracing();

	void setCamera(std::shared_ptr<Camera> camera);
	void setCollection(std::shared_ptr<HitableCollection> collection);

	void start();
	void endRender();
	void process(Window& win);

	void exportPic();

	bool isRunning() const;
};