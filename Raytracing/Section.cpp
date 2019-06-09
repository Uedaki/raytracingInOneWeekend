#include "Section.h"

#include "Camera.h"
#include "ctmRand.h"
#include "Hitable.h"
#include "HitableCollection.h"
#include "HitRecord.h"
#include "Material.h"
#include "Ray.h"

Section::Section()
	: _isRunning(false), _hasBecomeInactive(false)
	, _nx(0), _ny(0), _ns(0)
	, _idx(0), _size(0)
	, _thread(nullptr)
{}

Section::~Section()
{
	_isRunning = false;
	if (_thread && _thread->joinable())
	{
		_thread->join();
	}
}

void Section::defineGlobal(uint32_t nx, uint32_t ny, uint8_t ns)
{
	_nx = nx;
	_ny = ny;
	_ns = ns;
}

void Section::defineSection(uint32_t idx, uint32_t size)
{
	_idx = idx;
	_size = size;
}

void Section::start(std::vector<glm::vec3>& pic, std::shared_ptr<HitableCollection> collection, std::shared_ptr<Camera> cam)
{
	_isRunning = true;
	_collection = collection;
	_cam = cam;
	_thread = std::make_unique<std::thread>([this](std::vector<glm::vec3>* pic) { this->process(*pic); }, &pic);
}

void Section::access()
{
	_locker.lock();
}

void Section::endAccess()
{
	_locker.unlock();
}

uint32_t Section::getIdx() const
{
	return (_idx);
}

uint32_t Section::getSize() const
{
	return (_size);
}

bool Section::isActive() const
{
	return (_isRunning);
}

bool Section::hasBecomeInactive()
{
	if (_hasBecomeInactive)
	{
		if (_thread->joinable())
			_thread->join();
		_thread.reset(nullptr);

		_hasBecomeInactive = false;
		return (true);
	}
	return (false);
}

glm::vec3 Section::computeColor(const Ray& ray, const Hitable& world, int depth)
{
	HitRecord record;
	if (world.hit(ray, 0.001f, 100.0f, record))
	{
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 50 && record.material->scatter(ray, record, attenuation, scattered))
		{
			return (attenuation * computeColor(scattered, world, depth + 1));
		}
		else
			return (glm::vec3(0, 0, 0));
	}
	else
	{
		glm::vec3 direction = glm::normalize(ray.direction());
		float t = 0.5f * (direction.y + 1);
		return ((1 - t) * glm::vec3(1, 1, 1) + t * glm::vec3(0.5, 0.7, 1));
	}
}

void Section::process(std::vector<glm::vec3>& pic)
{
	for (uint32_t cs = 0; cs < _ns; ++cs)
	{
		for (uint32_t ci = _idx; ci < _idx + _size; ++ci)
		{
			int cx = ci % _nx;
			int cy = ci / _nx;

			float u = (static_cast<float>(cx) + ctmRand()) / static_cast<float>(_nx);
			float v = (static_cast<float>(cy) + ctmRand()) / static_cast<float>(_ny);
			Ray ray = _cam->getRay(u, v);

			glm::vec3 rgb = (static_cast<float>(cs) * pic[cx + cy * _nx] + computeColor(ray, *_collection, 0)) / (static_cast<float>(cs + 1));

			_locker.lock();
			pic[cx + cy * _nx] = rgb;

			if (!_isRunning)
			{
				_locker.unlock();
				return;
			}
			_locker.unlock();
		}
	}

	_locker.lock();
	_isRunning = false;
	_hasBecomeInactive = true;
	_locker.unlock();
}