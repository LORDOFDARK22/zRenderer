#include "InputManager.h"

namespace zRender
{
	InputManager::InputManager(Window& window) : window{ window }
	{
	}
	//Keyboard
	bool InputManager::GetKeyDown(Key key)
	{
		return glfwGetKey(window.GetWindow(), key) == GLFW_PRESS;
	}
	bool InputManager::GetKeyUp(Key key)
	{
		return glfwGetKey(window.GetWindow(), key) == GLFW_RELEASE;;
	}
	//Mouse
	zMath::Vector2 InputManager::GetMousePosition()
	{
		double x, y = 0;
		glfwGetCursorPos(window.GetWindow(), &x, &y);
		return zMath::Vector2{ (float)x, (float)y };
	}
	zMath::Vector2 InputManager::GetMouseScroller()
	{
		//glfwSetScrollCallback(window.GetWindow(), scroll_callback);

		return {};
	}
	bool InputManager::GetMouseButtonDown(MouseButton button)
	{
		return glfwGetMouseButton(window.GetWindow(), button) == GLFW_PRESS;
	}
	bool InputManager::GetMouseButtonUp(MouseButton button)
	{
		return glfwGetMouseButton(window.GetWindow(), button) == GLFW_RELEASE;
	}
	//Gamepad
	const char* InputManager::GetGamepadName(int gamepadID)
	{
		return glfwGetGamepadName(gamepadID);
	}
	bool InputManager::GetGamepadButtonDown(int gamepadID, GamepadButton button)
	{
		GLFWgamepadstate state;

		glfwGetGamepadState(gamepadID, &state);

		return state.buttons[button] == GLFW_PRESS;
	}
	bool InputManager::GetGamepadButtonUp(int gamepadID, GamepadButton button)
	{
		GLFWgamepadstate state;

		glfwGetGamepadState(gamepadID, &state);

		return state.buttons[button] == GLFW_RELEASE;
	}
	float InputManager::GetGamepadAxis(int gamepadID, GamepadAxis axis)
	{
		GLFWgamepadstate state;

		glfwGetGamepadState(gamepadID, &state);

		return state.axes[axis];
	}

}

