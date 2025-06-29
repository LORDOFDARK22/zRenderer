#pragma once
#include <string>
#include <SDL.h>

namespace EWorld
{
	struct WindowData
	{
		std::string title;
		int width, height;
		bool vSync;
		Uint32 flags;
	};

	class Window
	{
	private:
		WindowData window_data;
		SDL_Window* sdl_window;
		SDL_GLContext sdl_gl_context;
		SDL_Event sdl_event;
		bool isRunning = false;

	public:
		Window(const std::string& title, int width, int height, Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, bool vSync = true);
		~Window();

		//geters
		inline std::string& GetTitle() { return window_data.title; }
		inline int GetWidth() { return window_data.width; }
		inline int GetHeight() { return window_data.height; }
		inline bool IsVSync() { return window_data.vSync; }
		inline bool IsRunning() { return isRunning; }
		inline SDL_Window* GetSDLWindow() { return sdl_window; }
		//seters

		void SwapWindow();
		void Input();
		void UpdateViewport();
	};
}