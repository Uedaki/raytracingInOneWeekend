#pragma once

#include <glm/glm.hpp>
#include <Windows.h>

class Window
{
	HWND winHandler;

public:
	Window(HINSTANCE instance, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
	Window(const Window&) = delete;
	~Window() = default;

	void handleEvents();
	void updatePixel(int x, int y, const glm::vec3 &color);
	void render();

	bool isOpen() const;
};