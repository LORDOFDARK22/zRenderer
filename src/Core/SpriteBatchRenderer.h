#pragma once
#include <zMath/Vector2.h>
#include "../Essentials/Color.h"
#include "../Essentials/Font.h"

namespace zRender
{
	class SpriteBatchRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void Begin();
		static void End();
		static void Flush();

		static void DrawQuad(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, Color color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawText(Font* font, const std::string& text, const zMath::Vector2& position, const zMath::Vector2& scale, float size, float padding, float wrap, Color color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawSprite(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, Color color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawSpritesheet(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, int frameX, int frameY, int sheetCols, int sheetRows, Color color = { 1.0f, 1.0f, 1.0f, 1.0f });

		static int DrawCount;
		static int SpriteCount;

		static void RestStats();

	};
}