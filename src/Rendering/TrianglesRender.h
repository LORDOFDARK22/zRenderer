#pragma once
#include <iostream>
#include <vector>
#include <glm.hpp>
#include "Color.h"
#include "Shader.h"
#include "Camera.h"

namespace EWorld
{
	class TrianglesRender
	{
	private:
		uint32_t vertexArray, vertexBuffer, colorBuffer;
		std::vector<glm::vec2> vertices;
		std::vector<Color> colors;

	public:
		TrianglesRender();
		~TrianglesRender();

		void UpdateTrianglesOnGpu();

		void AddTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const Color& color);

		void Render(Shader* shader, Camera& camera);

	};
}