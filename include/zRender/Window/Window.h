#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <string>
#include <iostream>
#include "../Essentials/Color.h"
#include <zMath/Vector2.h>

#include <vector>

namespace zRender
{
	static float GetTime()
	{
		return (float)glfwGetTime();
	}

	static void InitZRender()
	{
		glfwInit();

		//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		std::cout << "[zRender]---success to Init\n";
	}

	static void SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	static void ClearsBuffers(bool color, bool depth, bool stencil)
	{
		glClearDepth(1.0f);
		glClear
		(
			(color ? GL_COLOR_BUFFER_BIT : 0) |
			(depth ? GL_DEPTH_BUFFER_BIT : 0) |
			(stencil ? GL_STENCIL_BUFFER_BIT : 0)
		);
	}

	static void ClearColor(zRender::Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	enum WindowHint
	{
		Window_Resizable_Hint = 0,
		Window_MaximiedHint = 1,
	};
	static void SetHint(WindowHint hint, bool value)
	{
		switch (hint)
		{
		case Window_Resizable_Hint:
			glfwWindowHint(GLFW_RESIZABLE, value);
			break;
		case Window_MaximiedHint:
			glfwWindowHint(GLFW_MAXIMIZED, value);
			break;
		}
	}

	class Window
	{
	private:
		GLFWwindow* window;
		int width, height;
		std::string title;
		bool isVsync;

	public:
		Window(int width, int height, const std::string& title, bool fullscreen = false);
		~Window();

		const int GetWidth() const { return width; }
		const int GetHeight() const { return height; }
		const std::string GetTitle() const { return title; }
		const bool IsVSync() const { return isVsync; }

		std::vector<std::string> GetDropFiles();
		void DropsClear();

		zMath::Vector2 GetWindowSize();

		GLFWwindow* GetWindow();

		bool WindowCloseShould();

		void SetTitle(const std::string& title);
		void SetVsync(bool value);
		void SetWindowIcon(const char* imagePath);
		void SetWindowIcon(const unsigned char* imageData, int length);
		void SetFullscreen(bool value);

		void Inputs();
		void SwapBuffers();
	};
}