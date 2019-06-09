#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class Camera;
class Hitable;
class HitableCollection;
class Ray;

class Section
{
	bool								_isRunning;
	bool								_hasBecomeInactive;

	uint32_t							_nx;
	uint32_t							_ny;
	uint8_t								_ns;

	uint32_t							_idx;
	uint32_t							_size;

	std::shared_ptr<Camera>				_cam;
	std::shared_ptr<HitableCollection>	_collection;

	std::unique_ptr<std::thread>		_thread;
	std::mutex							_locker;

public:
	Section();
	~Section();

	void defineGlobal(uint32_t nx, uint32_t ny, uint8_t ns);
	void defineSection(uint32_t idx, uint32_t size);

	void start(std::vector<glm::vec3>& pic, std::shared_ptr<HitableCollection> collection, std::shared_ptr<Camera> cam);

	void access(); // lock mutex to prevent the thread to modify pic
	void endAccess(); // unlock mutex
	
	uint32_t getIdx() const;
	uint32_t getSize() const;

	bool isActive() const;
	bool hasBecomeInactive();

	glm::vec3	computeColor(const Ray& ray, const Hitable& world, int depth);
	void		process(std::vector<glm::vec3>& pic);
};