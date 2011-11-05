////////////////////////////////////////////////////
// This file is part of NoLifeStory.              //
// Please see SuperGlobal.h for more information. //
////////////////////////////////////////////////////
#include "Global.h"

sf::Window* NLS::window = 0;
bool NLS::Graphics::NPOT = true;
bool NLS::Graphics::Shit = false;
bool NLS::Graphics::fullScreen = true;

void NLS::Graphics::Init(bool full) {
	window = new sf::Window(sf::VideoMode(800, 600), "NoLifeStory::Loading", full ? sf::Style::Fullscreen : sf::Style::Titlebar, sf::ContextSettings(0, 0, 0, 2, 0));
	glewExperimental = true;
	GLenum error = glewInit();
	switch (error) {
	case GLEW_OK:
		ucout << U("GLEW initialized") << endl;;
		break;
	case GLEW_ERROR_NO_GL_VERSION:
		ucerr << U("Unable to detect OpenGL version.");
		throw(273);
		break;
	case GLEW_ERROR_GL_VERSION_10_ONLY:
		ucerr << U("You only have OpenGL 1.0. What a fail.");
		throw(273);
		break;
	case GLEW_ERROR_GLX_VERSION_11_ONLY:
		ucerr << U("You only have GLX 1.1. What a fail.");
		throw(273);
		break;
	default:
		ucerr << U("GLEW has given me an unknown error code so I'll just abort.");
		throw(273);
		break;
	}
#ifdef NLS_WINDOWS
	HANDLE hIcon = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_NOLIFESTORY_ICON));
	HWND hWnd = window->GetSystemHandle();
	if (hWnd) {
		::SendMessageW(hWnd, WM_SETICON, ICON_SMALL, (LPARAM) hIcon);
	}
#endif
	window->ShowMouseCursor(false);
	window->EnableKeyRepeat(true);
	glClearColor(0, 0, 0, 0); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	window->Display();
	if (!GLEW_VERSION_1_2) {
		ucerr << U("Seriously. You don't have OpenGL 1.2? Maybe you forgot to install drivers for your gpu? Wait a second, do you even HAVE a gpu? Seriously, get a job and buy a graphics card.") << endl;
		Shit = true;
	}
	if (!GLEW_ARB_texture_non_power_of_two and !GLEW_VERSION_2_0) {
		ucerr << U("Missing support for NPOT textures. Using slower backward's compatible code. Please upgrade your graphics card and/or drivers when you get a chance.") << endl;
		NPOT = false;
	}
}

void NLS::Graphics::setFullScreen() {
	if (fullScreen == true) fullScreen = false;
	else fullScreen = true;
	window->Close();
	Init(fullScreen);
}


void NLS::Graphics::Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	View.Step();
	Map::Draw();
	Foothold::Draw();
	View.Reset();
	Time.Step();
	ustring title = U("NoLifeStory::FrameRate = ")+tostring((int)Time.fps);//Fix this SFML!
#ifdef NLS_WINDOWS
	window->SetTitle(string(title.begin(), title.end()));
#else
	window->SetTitle(title);
#endif
	window->Display();
#ifdef DEBUG
	switch (glGetError()) {
	case GL_NO_ERROR:
		break;
	default:
		ucerr << U("OH GOD OPENGL FAILED") << endl;
	}
#endif
}