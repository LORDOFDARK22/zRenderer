#include "Camera.h"

EWorld::Camera::Camera(const glm::vec2& position, int window_width, int window_height, float scale) : position{position}, scale{scale}
{
	projection = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, -1.0f, 100.0f);
	//view = glm::lookAt(position, )
}

void EWorld::Camera::Update(int window_width, int window_height)
{
	projection = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, -1.0f, 100.0f);
}
