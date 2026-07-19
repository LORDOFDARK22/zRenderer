#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <string>
#include <sstream>
#include <iostream>
#include "../Essentials/Color.h"
#include <zMath/Vector2.h>

#include <vector>

#define ZRAssert(x, ...) {if (x){ std::cout << "Failed: " << __VA_ARGS__ << std::endl; __debugbreak();}}

namespace zRender
{
	/*! @brief Returns the zRender time.
	*
	*  This function returns the current zRender time, in seconds.  Unless the time
	*  has been set using @ref SetTime it measures time elapsed since zRender was
	*  initialized.
	*
	*  This function and @ref SetTime are helper functions on top of @ref
	*
	*  The resolution of the timer is system dependent, but is usually on the order
	*  of a few micro- or nanoseconds.  It uses the highest-resolution monotonic
	*  time source on each operating system.
	*
	*  @return The current time, in seconds, or zero if an
	*  [error](@ref error_handling) occurred.
	*
	*  @errors Possible errors include @ref GLFW_NOT_INITIALIZED.
	*
	*  @thread_safety This function may be called from any thread.  Reading and
	*  writing of the internal base time is not atomic, so it needs to be
	*  externally synchronized with calls to @ref SetTime.
	*
	*  @sa @ref time
	*/
	static float GetTime()
	{
		return (float)glfwGetTime();
	}

	static std::string GetVendor()
	{
		std::stringstream ss;
		ss << glGetString(GL_VENDOR);
		return ss.str();
	}
	static std::string GetRenderer()
	{
		std::stringstream ss;
		ss << glGetString(GL_RENDERER);
		return ss.str();
	}
	static std::string GetVersion()
	{
		std::stringstream ss;
		ss << glGetString(GL_VERSION);
		return ss.str();
	}

	/*! @brief Initializes the GLFW library.
 *
 *  This function initializes the GLFW library.  Before most GLFW functions can
 *  be used, GLFW must be initialized, and before an application terminates GLFW
 *  should be terminated in order to free any resources allocated during or
 *  after initialization.
 *
 *  If this function fails, it calls @ref glfwTerminate before returning.  If it
 *  succeeds, you should call @ref glfwTerminate before the application exits.
 *
 *  Additional calls to this function after successful initialization but before
 *  termination will return `GLFW_TRUE` immediately.
 *
 *  The @ref GLFW_PLATFORM init hint controls which platforms are considered during
 *  initialization.  This also depends on which platforms the library was compiled to
 *  support.
 *
 *  @return `GLFW_TRUE` if successful, or `GLFW_FALSE` if an
 *  [error](@ref error_handling) occurred.
 *
 *  @errors Possible errors include @ref GLFW_PLATFORM_UNAVAILABLE and @ref
 *  GLFW_PLATFORM_ERROR.
 *
 *  @remark @macos This function will change the current directory of the
 *  application to the `Contents/Resources` subdirectory of the application's
 *  bundle, if present.  This can be disabled with the @ref
 *  GLFW_COCOA_CHDIR_RESOURCES init hint.
 *
 *  @remark @macos This function will create the main menu and dock icon for the
 *  application.  If GLFW finds a `MainMenu.nib` it is loaded and assumed to
 *  contain a menu bar.  Otherwise a minimal menu bar is created manually with
 *  common commands like Hide, Quit and About.  The About entry opens a minimal
 *  about dialog with information from the application's bundle.  The menu bar
 *  and dock icon can be disabled entirely with the @ref GLFW_COCOA_MENUBAR init
 *  hint.
 *
 *  @remark __Wayland, X11:__ If the library was compiled with support for both
 *  Wayland and X11, and the @ref GLFW_PLATFORM init hint is set to
 *  `GLFW_ANY_PLATFORM`, the `XDG_SESSION_TYPE` environment variable affects
 *  which platform is picked.  If the environment variable is not set, or is set
 *  to something other than `wayland` or `x11`, the regular detection mechanism
 *  will be used instead.
 *
 *  @remark @x11 This function will set the `LC_CTYPE` category of the
 *  application locale according to the current environment if that category is
 *  still "C".  This is because the "C" locale breaks Unicode text input.
 *
 *  @thread_safety This function must only be called from the main thread.
 *
 *  @sa @ref glfwInitHint
 *  @sa @ref glfwTerminate
 */
	static void InitZRender()
	{
		ZRAssert(glfwInit() == GLFW_FALSE, "Init zRender!");

		//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	static void SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	static void ClearsBuffers(bool color, bool depth, bool stencil)
	{
		if (depth) glClearDepth(1.0f);
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

	/*This basic Window can control and CrossPlatform*/
	class Window
	{
	private:
		GLFWwindow* window;
		int width, height;
		std::string title;
		bool isVsync;

	public:
		Window();
		Window(int width, int height, const std::string& title, bool fullscreen = false);
		~Window();

		//! @brief Returns the current Window width size.
		const int GetWidth() const { return width; }
		//! @brief Returns the current Window height size.
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

		void Update();
	};
}