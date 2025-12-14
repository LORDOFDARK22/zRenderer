#pragma once
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/ElementBuffer.h"
#include "../Essentials/Texture.h"
#include "../Essentials/Shader.h"
#include "Camera2D.h"

#include <zMath/Vector2.h>

namespace zRender
{
	class SpritesRender
	{
	private:
		VertexArray VAO;
		VertexBuffer VBO;
		std::vector<float> sprites;
		std::vector<uint32_t> textures;
		Shader* currentShader;
		Camera2D* currentCamera;

	public:
		SpritesRender();
		~SpritesRender();

		void Begin();
		void End();

		void DrawSprite(uint32_t textureID, const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, Color color);
		void DrawSpritesheet(uint32_t textureID, int frameX, int frameY, int sheetCols, int sheetRows, const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, Color color);

		void Render(Shader* shader, Camera2D& camera);

	};
}