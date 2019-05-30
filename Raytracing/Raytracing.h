#pragma once

#include <array>
#include <chrono>
#include <glm/glm.hpp>

#include "Section.h"

class Camera;
class HitableCollection;
class Window;

typedef std::chrono::time_point<std::chrono::steady_clock> Clock;

class Raytracing
{
	bool					_isRunning;

	uint32_t				_nx;
	uint32_t				_ny;
	uint8_t					_ns;

	glm::vec3*				_pic;

	Camera*					_cam;
	HitableCollection*		_collection;

	uint8_t					_activeSection;
	std::array<Section, 4>	_sections;

	Clock					_startTime;

public:
	Raytracing(uint32_t width, uint32_t height, uint8_t nbSamples);

	void setCamera(Camera* camera);
	void setCollection(HitableCollection* collection);

	void start();
	void endRender();
	void process(Window& win);

	void exportPic();

	bool isRunning() const;
};