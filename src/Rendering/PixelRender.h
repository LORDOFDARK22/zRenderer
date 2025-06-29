#pragma once
#include <glm.hpp>
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Color.h"

namespace EWorld
{
	class PixelRender
	{
	private:
		uint32_t vertexArray, vertexBuffer, colorBuffer;
		std::vector<glm::vec2> points;
		std::vector<Color> colors;

	public:
		PixelRender();
		~PixelRender();

		void AddPixel(const glm::vec2& pos, const Color& color);

		void UpdatePixelsOnGPU();

		void Render(Shader* shader, Camera& camera);
	};
}