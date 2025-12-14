#include "Window.h"
#include "../../stb_image.h"

namespace zRender
{
	static std::vector<std::string> g_DroppedFiles;

	void DropCallback(GLFWwindow* window, int count, const char** paths)
	{
		g_DroppedFiles.clear();
		for (int i = 0; i < count; i++)
		{
			g_DroppedFiles.push_back(paths[i]);
			//std::cout << "Dropped file: " << paths[i] << std::endl;
		}

	}
	Window::Window(int width, int height, const std::string& title, bool fullscreen) : width{width}, height{height}, title{title}
	{
		if (fullscreen)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			window = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		else
		{
			window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		}
		if (window == NULL)
		{
			std::cout << "[zRender]---Failed to create Window!" << std::endl;
		}
		glfwMakeContextCurrent(window);
		gladLoadGL();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/*glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);*/

		glfwSetDropCallback(window, DropCallback);

		std::cout << "[zRender]---success to Create Window { width: " << width << ", height: " << height << ", title: " << title << " }" << std::endl;
	}
	Window::~Window()
	{
		std::cout << "[zRender]---Window is Closed!\n";
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	std::vector<std::string> Window::GetDropFiles()
	{
		return g_DroppedFiles;
	}
	void Window::DropsClear()
	{
		g_DroppedFiles.clear();
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
	void Window::Inputs()
	{
		glfwPollEvents();
	}
	void Window::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

	void Window::SetWindowIcon(const char* imagePath)
	{
		int width, height, channels;
		unsigned char* pixels = stbi_load(imagePath, &width, &height, &channels, STBI_rgb_alpha); // 4 for RGBA

		GLFWimage images[1];
		images[0].width = width;
		images[0].height = height;
		images[0].pixels = pixels;

		glfwSetWindowIcon(window, 1, images);
		stbi_image_free(pixels);
	}
	void Window::SetWindowIcon(const unsigned char* imageData, int length)
	{
		int width, height, channels;
		unsigned char* data = stbi_load_from_memory(imageData, length, &width, &height, &channels, STBI_rgb_alpha);

		GLFWimage images[1];
		images[0].width = width;
		images[0].height = height;
		images[0].pixels = data;

		glfwSetWindowIcon(window, 1, images);
		stbi_image_free(data);
	}
	void Window::SetFullscreen(bool value)
	{
		if (value)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
		}
		else
		{
			glfwSetWindowMonitor(window, NULL, 100, 100, width, height, 0);
		}
	}
}
