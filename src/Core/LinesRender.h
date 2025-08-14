#pragma once
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Essentials/Color.h"
#include "Camera2D.h"
#include "../Essentials/Shader.h"

#include <vector>
#include <zMath/Vector2.h>

namespace zRender
{
	class LinesRender
	{
	private:
		VertexArray VAO;
		VertexBuffer VBO;
		std::vector<float> lines;

	public:
		LinesRender();
		LinesRender(float lineWidth);
		~LinesRender();

		void Begin();
		void End();

		void DrawLine(const zMath::Vector2& start, const zMath::Vector2& end, Color color);

		void Render(Shader* shader, Camera2D& camera);
	};
}