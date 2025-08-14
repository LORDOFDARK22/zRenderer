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
	class CirclesRender
	{
	private:
		VertexArray VAO;
		VertexBuffer VBO;
		VertexBuffer IVBO;
		std::vector<float> circles;
		bool line = false;

	public:
		CirclesRender(float lineWidth);
		CirclesRender();
		~CirclesRender();

		void Begin();
		void End();

		void DrawCircle(const zMath::Vector2& position, float radius, Color color);

		void Render(Shader* shader, Camera2D& camera);

	};
}