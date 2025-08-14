#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <string>
#include <iostream>
#include "../Essentials/Color.h"
#include <zMath/Vector2.h>

namespace zRender
{
	static float GetTime()
	{
		return (float)glfwGetTime();
	}

	static void InitZRender()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		std::cout << "[zRender]---success to Init\n";
	}

	static void ClearColor(zRender::Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		Window(int width, int height, const std::string& title);
		~Window();

		const int GetWidth() const { return width; }
		const int GetHeight() const { return height; }
		const std::string GetTitle() const { return title; }
		const bool IsVSync() const { return isVsync; }

		zMath::Vector2 GetWindowSize();

		GLFWwindow* GetWindow();

		bool WindowCloseShould();

		void SetTitle(const std::string& title);
		void SetVsync(bool value);
		void SetViewport(int x, int y, int width, int height);
		void SetWindowIcon(int count, const char* imagePath);

		void Inputs();
		void SwapBuffers();
	};
}