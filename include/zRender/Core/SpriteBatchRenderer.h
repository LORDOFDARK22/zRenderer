#pragma once
#include <zMath/Vector2.h>
#include <zMath/Vector3.h>
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

		static void DrawQuad(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, Color color = WHITE);
		static void DrawText(Font* font, const std::string& text, const zMath::Vector2& position, const zMath::Vector2& scale, float size, float padding, float wrap, int layer = 0, Color color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawSprite(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, int layer = 0, bool flipX = false, bool flipY = false, Color color = WHITE);
		static void DrawSpritesheet(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, int frameX, int frameY, int sheetCols, int sheetRows, int layer = 0, bool flipX = false, bool flipY = false, Color color = WHITE);

		static void DrawSpritePro(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, const zMath::Vector2& uv0 = {0,0}, const zMath::Vector2& uv1 = {1,1}, int layer = 0, bool flipX = false, bool flipY = false, Color color = WHITE);


		static int DrawCount;
		static int SpriteCount;
		static int SpritesheetCount;
		static int TextCount;

		static int IndicesCount;
		static int VerticesCount;

		static void RestStats();

	};
}