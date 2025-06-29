#pragma once
#include <iostream>
#include <vector>
#include <glm.hpp>
#include "Color.h"
#include "Shader.h"
#include "Camera.h"

namespace EWorld
{
	class Triangle
	{
	private:
		uint32_t vertexArray, vertexBuffer, colorBuffer;
		glm::vec2 position;
		glm::vec2 scale;
		float angle;
		Color color;

		glm::mat4 GetModelMatrix();

	public:
		Triangle(const glm::vec2& position, const glm::vec2& scale, float angle, const Color& color);
		~Triangle();

		inline void SetAngle(float angle) { this->angle = angle; }

		void Render(Shader* shader, Camera& camera);

	};
}