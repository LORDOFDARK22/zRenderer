#pragma once
#include <vector>
#include <zMath/Vector2.h>

#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Essentials/Color.h"
#include "Camera2D.h"
#include "../Essentials/Shader.h"

namespace zRender
{
	class PixelsRender
	{
	private:
		VertexArray VAO;
		VertexBuffer VBO;
		std::vector<float> pixels;

	public:
		PixelsRender();
		PixelsRender(float pixelSize);
		~PixelsRender();

		void Begin();
		void End();

		void DrawPixel(const zMath::Vector2& position, Color color);

		void Render(Shader* shader, Camera2D& camera);

	};
}