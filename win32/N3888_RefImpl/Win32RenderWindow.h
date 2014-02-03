
#ifndef _WIN32RENDERWINDOW_
#define _WIN32RENDERWINDOW_

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <string>
#include "drawing.h"
#include <memory>

class Win32RenderWindow
{
public:

	Win32RenderWindow( unsigned int width, unsigned int height, const std::wstring& caption );
	~Win32RenderWindow();

	HWND GetHandle();

	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void ShowSaveAsPNGDialog();

	std::unique_ptr<std::experimental::drawing::surface> g_psurface;

private:
	HWND handle;
};

#endif // _WIN32RENDERWINDOW_