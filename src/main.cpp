#define SDL_MAIN_HANDLED 1;
#include <iostream>
#include <SDL.h>
#include <glm.hpp>
#include <glad/glad.h>

#include "Rendering/Window.h"

#include "Rendering/PixelRender.h"
#include "Rendering/LinesRender.h"
#include "Rendering/TrianglesRender.h"

#include "Rendering/Triangle.h"
#include "Rendering/Sprite.h"

#include "Rendering/Shader.h"
#include "Rendering/Camera.h"

constexpr int WINDOW_WIDTH = 1270;
constexpr int WINDOW_HEIGHT = 720;

int main()
{
	EWorld::Window window{ "EWolrd", WINDOW_WIDTH, WINDOW_HEIGHT };

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Camera
	EWorld::Camera camera{ glm::vec2{500.0f, 0.0f}, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f };

	std::vector<EWorld::Sprite> sprites;

	//Renderer
	/*for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			sprites.push_back(EWorld::Sprite{ "Assets/Textures/texture.jpg", glm::vec2{x * 6, y * 6}, glm::vec2{5.0f, 5.0f}, -1.55f, EWorld::Color{ 1.0f, 0.0f, 0.0f, 1.0f } });
		}
	}*/

	EWorld::Sprite sprite{ "Assets/Textures/texture.jpg", glm::vec2{400.0f, 300.0f}, glm::vec2{150.0f, 150.0f}, 0.0f, EWorld::Color{ 1.0f, 0.0f, 0.0f, 1.0f } };

	//Shader
	EWorld::Shader shader{ };
	shader.Create("Assets/Shaders/sprite.vert", "Assets/Shaders/sprite.frag");

	while (window.IsRunning())
	{
		window.Input();

		//UpdateSizeWindow for balunced
		window.UpdateViewport();
		camera.Update(window.GetWidth(), window.GetHeight());

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		//Draw
		sprite.Render(&shader, camera);
		/*for (auto sprite : sprites)
		{
			sprite.Render(&shader, camera);
		}*/

		window.SwapWindow();
	}

	return 0;
}



//// 16384 triangles draw per 10fps | gpu vram 1900mb
//for (int x = 0; x < 128; x++)
//{
//	for (int y = 0; y < 128; y++)
//	{
//		triangles.push_back(new EWorld::Triangle{ glm::vec2{x * 7, y * 7}, glm::vec2{6.0f, 6.0f},  1.0f, EWorld::Color{ 1.0f, 1.0f, 0.0f, 1.0f } });
//	}
//}