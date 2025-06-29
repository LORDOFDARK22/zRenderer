#pragma once
#include <iostream>
#include <vector>
#include <glm.hpp>
#include "Shader.h"
#include "Color.h"
#include "Camera.h"

namespace EWorld
{
	class LinesRender
	{
	private:
		uint32_t vertexArray, vertexBuffer, colorBuffer;
		std::vector<glm::vec2> vertices;
		std::vector<Color> colors;

	public:
		LinesRender();
		~LinesRender();

		void AddLine(const glm::vec2& start, const glm::vec2& end, const Color& startColor, const Color& endColor);

		void UpdateLinesOnGpu();

		void Render(Shader* shader, Camera& camera);
	};
}