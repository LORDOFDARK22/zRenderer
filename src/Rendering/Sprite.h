#pragma once
#include <iostream>
#include <vector>
#include <glm.hpp>
#include "Color.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

namespace EWorld
{
	class Sprite
	{
	private:
		uint32_t vertexArray, vertexBuffer, ebo, colorBuffer;
		glm::vec2 position;
		Color color;
		Texture* texture;
		glm::vec2 scale;
		float angle;

		glm::mat4 GetModelMatrix();

	public:
		Sprite(const char* imagePath, const glm::vec2& position, const glm::vec2& scale, float angle, const Color& color);//, const glm::vec2& scale, float angle
		~Sprite();

		//inline void SetAngle(float angle) { this->angle = angle; }

		void Render(Shader* shader, Camera& camera);
	};
}