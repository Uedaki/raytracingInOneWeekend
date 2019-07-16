#include "Raytracing.h"

#include <fstream>
#include <string>

#include "Camera.h"
#include "HitableCollection.h"
#include "Window.h"

Raytracing::Raytracing(uint32_t width, uint32_t height, uint8_t nbSamples)
	: _isRunning(false), _nx(width), _ny(height), _ns(nbSamples), _pic(width * height)
{
	memset(_pic.data(), 0, width * height * sizeof(glm::vec3));

	uint32_t currStep = 0;
	const uint32_t step = (width * height) / static_cast<uint32_t>(_sections.size());
	for (Section& section : _sections)
	{
		section.defineGlobal(width, height, nbSamples);
		section.defineSection(currStep, step);
		currStep += step;
	}
}

Raytracing::~Raytracing()
{
	OutputDebugStringA("Stopping...\n");
	if (_isRunning)
	{
		endRender();
	}
}

void Raytracing::setCamera(std::shared_ptr<Camera> camera)
{
	_cam = camera;
}

std::shared_ptr<Camera> Raytracing::getCamera()
{
	return (_cam);
}

void Raytracing::setCollection(std::shared_ptr<HitableCollection> collection)
{
	_collection = collection;
}

std::shared_ptr<HitableCollection> Raytracing::getCollection()
{
	return (_collection);
}

void Raytracing::start()
{
	for (Section& section : _sections)
	{
		section.start(_pic, _collection, _cam);
	}
	_activeSection = static_cast<uint8_t>(_sections.size());
	_startTime = std::chrono::steady_clock::now();
	_isRunning = true;
}

void Raytracing::endRender()
{
	Clock endTime = std::chrono::steady_clock::now();
	uint32_t total = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::seconds>(endTime - _startTime).count());
	uint32_t hours = total / 60 / 60;
	uint32_t minutes = (total / 60) % 60;
	uint32_t seconds = total % 60;

	OutputDebugStringA("Rendering finished in ");
	OutputDebugStringA(std::to_string(hours).c_str());
	OutputDebugStringA("hours ");
	OutputDebugStringA(std::to_string(minutes).c_str());
	OutputDebugStringA("minutes ");
	OutputDebugStringA(std::to_string(seconds).c_str());
	OutputDebugStringA("seconds\n");

	exportPic();
	_isRunning = false;
}

void Raytracing::process(Window& win)
{
	if (!_isRunning)
		return;

	if (_activeSection == 0)
	{
		endRender();
		return;
	}

	for (Section& section : _sections)
	{
		int start = section.getIdx();
		int size = section.getSize();

		section.access();
		if (!section.isActive())
		{
			if (!section.hasBecomeInactive())
			{
				section.endAccess();
				continue;
			}
			_activeSection -= 1;
		}
		for (int i = start; i < start + size; ++i)
		{
			glm::vec3 rgb = _pic[i];
			glm::vec3 color = 255.99f * glm::vec3(sqrt(rgb[0]), sqrt(rgb[1]), sqrt(rgb[2]));
			win.updatePixel(i % _nx, i / _nx, color);
		}
		section.endAccess();
	}
}

void Raytracing::exportPic()
{
	std::ofstream file("output.ppm");
	if (file)
	{
		file << "P3\n"
			<< _nx << " " << _ny << "\n"
			<< "255\n";

		for (uint32_t j = _ny - 1; j < _ny; --j) // this loop check for j < _ny because when as j is unsigned -1 will be the highest value j can hold, therefore higher than _ny
		{
			for (uint32_t i = 0; i < _nx; ++i)
			{
				int r = static_cast<int>(255.99 * _pic[j * _nx + i].r);
				int g = static_cast<int>(255.99 * _pic[j * _nx + i].g);
				int b = static_cast<int>(255.99 * _pic[j * _nx + i].b);
				file << r << " " << g << " " << b << "\n";
			}
		}
		file.close();
	}
}

bool Raytracing::isRunning() const
{
	return (_isRunning);
}