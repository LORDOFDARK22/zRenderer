#pragma once
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "Camera2D.h"
#include "../Essentials/Shader.h"

#include <vector>

namespace zRender
{
	class TrianglesRender
	{
	private:
		VertexArray VAO;
		VertexBuffer VBO;
		std::vector<float> triangles;

	public:
		TrianglesRender();
		~TrianglesRender();

		void Begin();
		void End();

		void DrawTriangle(const zMath::Vector2& v1, const zMath::Vector2& v2, const zMath::Vector2& v3, Color color);

		void Render(Shader* shader, Camera2D& camera);

	};
}