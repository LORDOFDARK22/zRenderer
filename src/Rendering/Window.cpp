#include "Window.h"
#include <iostream>
#include <glad/glad.h>

namespace EWorld
{
	Window::Window(const std::string& title, int width, int height, Uint32 flags, bool vSync) : window_data{.title = title, .width = width, .height = height, .vSync = vSync, .flags = flags}
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::cout << "failed to init SDL2!" << std::endl;
		}

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		sdl_window = SDL_CreateWindow(window_data.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_data.width, window_data.height, window_data.flags);
		if (!sdl_window)
		{
			std::cout << "failed to create Window!" << std::endl;
			return;
		}

		sdl_gl_context = SDL_GL_CreateContext(sdl_window);
		if (!sdl_gl_context)
		{
			std::cout << "failed to create Context for window!" << std::endl;
			return;
		}

		if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		{
			std::cout << "failed to Load glad!" << std::endl;
			return;
		}

		isRunning = true;
	}

	Window::~Window()
	{
		SDL_GL_DeleteContext(sdl_gl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
	}
	void Window::SwapWindow()
	{
		SDL_GL_SwapWindow(sdl_window);
	}
	void Window::Input()
	{
		SDL_PollEvent(&sdl_event);

		switch (sdl_event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_WINDOWEVENT:
			if (sdl_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				// Window size has changed
				int newWidth = sdl_event.window.data1;
				int newHeight = sdl_event.window.data2;

				window_data.width = newWidth;
				window_data.height = newHeight;
			}
			break;
		}
	}
	void Window::UpdateViewport()
	{
		glViewport(0, 0, window_data.width, window_data.height);
	}
}

