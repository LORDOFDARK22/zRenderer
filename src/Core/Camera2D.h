#pragma once
#include <zMath/Matrix4x4.h>
#include <zMath/Vector2.h>

namespace zRender
{
	class Camera2D
	{
	private:
		zMath::Vector2 position, screenOffset;
		float scale;
		zMath::Matrix4x4 view, projection;
		int width, height;
		bool needsUpdate;

	public:
		Camera2D(int width, int height);
		
		inline const zMath::Vector2 GetPosition() const { return position; }
		inline const zMath::Vector2 GetScreenOffset() const { return screenOffset; }
		inline const int GetWidth() const { return width; }
		inline const int GetHeight() const { return height; }
		inline const float GetScale() const { return scale; }

		inline zMath::Matrix4x4 GetView() { return view; }
		inline zMath::Matrix4x4 GetProjection() { return projection; }

		inline void SetPosition(const zMath::Vector2& pos) { position = pos; needsUpdate = true; }
		inline void SetScale(float scl) { scale = scl; needsUpdate = true; }

		void SetWindowSize(int width, int height);

		zMath::Vector2 ScreenCoordsToWolrd(const zMath::Vector2& screenCoords);
		zMath::Vector2 WorldCoordsToScreen(const zMath::Vector2& worldCoords);

		void Update();
	};
}