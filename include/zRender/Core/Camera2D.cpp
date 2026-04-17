#include "Camera2D.h"

namespace zRender
{
	Camera2D::Camera2D(int width, int height) : width{ width }, height{ height }, scale{ 1.0f }, position{ 0.0f, 0.0f }, screenOffset{ 0.0f, 0.0f }, view{ zMath::Matrix4x4::identity }, needsUpdate { true }
	{
		projection = zMath::Matrix4x4::Ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

		//projection = zMath::Matrix4x4::Perspective(45.0f, static_cast<float>(width) / static_cast<float>(height), -1.0f, 100.0f);

		Update();
	}
	void Camera2D::SetWindowSize(int width, int height)
	{
		this->width = width;
		this->height = height;

		projection = zMath::Matrix4x4::Ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
	}
	zMath::Vector2 Camera2D::ScreenCoordsToWolrd(const zMath::Vector2& screenCoords)
	{
		zMath::Vector2 worldCoords{ screenCoords };

		worldCoords -= screenOffset;
		worldCoords /= scale;
		worldCoords += position;

		return worldCoords;
	}
	zMath::Vector2 Camera2D::WorldCoordsToScreen(const zMath::Vector2& worldCoords)
	{
		zMath::Vector2 screenCoords{ worldCoords };

		screenCoords += screenOffset;
		screenCoords *= scale;
		screenCoords -= position;

		return screenCoords;
	}
	void Camera2D::Update()
	{
		if (!needsUpdate)
			return;

		//Translate
		zMath::Vector3 translate{ -position.x + screenOffset.x, -position.y + screenOffset.y, 0.f };
		zMath::Vector3 scl{ scale, scale, 0.0f };

		view = zMath::Matrix4x4::Translate(translate) * zMath::Matrix4x4::Scale(scl);

		needsUpdate = false;
	}
}
