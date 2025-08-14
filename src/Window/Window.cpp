#include "Window.h"
//#include "../stb_image.h"

namespace zRender
{
	Window::Window(int width, int height, const std::string& title) : width{width}, height{height}, title{title}
	{
		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (window == NULL)
		{
			std::cout << "[zRender]---Failed to create Window!" << std::endl;
		}
		glfwMakeContextCurrent(window);
		gladLoadGL();

		//glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::cout << "[zRender]---success to Create Window { width: " << width << ", height: " << height << ", title: " << title << " }" << std::endl;
	}
	Window::~Window()
	{
		std::cout << "[zRender]---Window is Closed!\n";
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	zMath::Vector2 Window::GetWindowSize()
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		return zMath::Vector2{(float)width, (float)height};
	}
	GLFWwindow* Window::GetWindow()
	{
		return window;
	}
	bool Window::WindowCloseShould()
	{
		return glfwWindowShouldClose(window);
	}
	void Window::SetTitle(const std::string& title)
	{
		this->title = title;
		glfwSetWindowTitle(window, title.c_str());
	}
	void Window::SetVsync(bool value)
	{
		isVsync = value;
		glfwSwapInterval(isVsync);
	}
	void Window::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}
	void Window::Inputs()
	{
		glfwPollEvents();
	}
	void Window::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

	void Window::SetWindowIcon(int count, const char* imagePath)
	{
		//int width, height, channels;
		//unsigned char* pixels = stbi_load(imagePath, &width, &height, &channels, 4); // 4 for RGBA

		//GLFWimage images[1];
		//images[0].width = width;
		//images[0].height = height;
		//images[0].pixels = pixels;

		//glfwSetWindowIcon(window, count, images);
		//stbi_image_free(pixels);
	}
}
