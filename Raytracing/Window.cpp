#include "Window.h"

#include <stdexcept>

namespace
{
	bool		gIsOpen = false;
	BITMAPINFO	gBitmapInfo;
	void*		gBitmapBuffer;
	int			gBitmapWidth;
	int			gBitmapHeight;

	static void windowResizeBuffer(int width, int height)
	{
		if (gBitmapBuffer)
		{
			VirtualFree(gBitmapBuffer, 0, MEM_RELEASE);
		}

		gBitmapWidth = width;
		gBitmapHeight = height;

		gBitmapInfo.bmiHeader.biSize = sizeof(gBitmapInfo.bmiHeader);
		gBitmapInfo.bmiHeader.biWidth = width;
		gBitmapInfo.bmiHeader.biHeight = height;
		gBitmapInfo.bmiHeader.biPlanes = 1;
		gBitmapInfo.bmiHeader.biBitCount = 32;
		gBitmapInfo.bmiHeader.biCompression = BI_RGB;
		gBitmapInfo.bmiHeader.biSizeImage = 0;
		gBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
		gBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
		gBitmapInfo.bmiHeader.biClrUsed = 0;
		gBitmapInfo.bmiHeader.biClrImportant = 0;

		if (!(gBitmapBuffer = VirtualAlloc(nullptr, width * height * 4, MEM_COMMIT, PAGE_READWRITE)))
			throw (std::runtime_error("Unable to allocate the window buffer"));
	}

	static void windowUpdate(HDC deviceContext, RECT& windowRect)
	{
		int windowWidth = windowRect.right - windowRect.left;
		int windowHeight = windowRect.bottom - windowRect.top;
		StretchDIBits(
			deviceContext,
			0, 0, gBitmapWidth, gBitmapHeight,
			0, 0, windowWidth, windowHeight,
			gBitmapBuffer,
			&gBitmapInfo,
			DIB_RGB_COLORS,
			SRCCOPY);
	}

	LRESULT CALLBACK windowCallback(
		HWND   win,
		UINT   msg,
		WPARAM wParam,
		LPARAM lParam)
	{
		LRESULT result = 0;

		switch (msg)
		{
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(win, &rect);
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			windowResizeBuffer(width, height);
			break;
		}

		case WM_DESTROY:
		{
			gIsOpen = false;
			break;
		}

		case WM_CLOSE:
		{
			gIsOpen = false;
			break;
		}

		case WM_PAINT:
		{
			RECT rect;
			PAINTSTRUCT paint;
			HDC deviceContext = BeginPaint(win, &paint);
			GetClientRect(win, &rect);
			windowUpdate(deviceContext, rect);
			EndPaint(win, &paint);
			break;
		}

		default:
		{
			result = DefWindowProc(win, msg, wParam, lParam);
			break;
		}
		}
		return (result);
	}
}

Window::Window(HINSTANCE instance, int width, int height)
{
	width += 16;// complete the size to have an active window with the desired size
	height += 39; // complete the size to have an active window with the desired size

	WNDCLASS wc = {};
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = windowCallback;
	wc.hInstance = instance;
	wc.lpszClassName = "Path tracing";
	if (!RegisterClass(&wc))
		throw (std::runtime_error("Unable to generate window context"));

	winHandler = CreateWindowEx(
		0,
		wc.lpszClassName,
		"Path tracing",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		width, // width
		height, // height
		0,
		0,
		instance,
		0);
	if (!winHandler)
		throw (std::runtime_error("Unable to create a window"));

	gIsOpen = true;
}

void Window::updatePixel(int x, int y, const glm::vec3& color)
{
	if (x >= gBitmapWidth || y >= gBitmapHeight)
		return;

	uint8_t padding = gBitmapInfo.bmiHeader.biBitCount / 8;
	uint8_t* bitmapColor = static_cast<uint8_t*>(gBitmapBuffer);
	bitmapColor += gBitmapWidth * padding * y + x * padding;
	bitmapColor[0] = static_cast<int>(color[2]);
	bitmapColor[1] = static_cast<int>(color[1]);
	bitmapColor[2] = static_cast<int>(color[0]);
}

void Window::handleEvents()
{
	MSG msg;
	while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			gIsOpen = false;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
}

void Window::render()
{
	RECT rect;
	HDC deviceContext = GetDC(winHandler);
	GetClientRect(winHandler, &rect);
	windowUpdate(deviceContext, rect);
	ReleaseDC(winHandler, deviceContext);
}

bool Window::isOpen() const
{
	return (gIsOpen);
}