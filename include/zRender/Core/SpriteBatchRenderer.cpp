#include "SpriteBatchRenderer.h"

#include <glad/glad.h>
#include <gl/GL.h>
#include <array>

#include <zMath/Vector3.h>
#include <zMath/Math.h>
#include <algorithm>

namespace zRender
{
	static const size_t MaxSpriteCount = 10000;
	static const size_t MaxVertexCount = MaxSpriteCount * 4;
	static const size_t MaxIndexCount = MaxSpriteCount * 6;
	static const size_t MaxTextures = 32;

	struct Sprite
	{
		zMath::Vector2 position;
		Color color;
		zMath::Vector2 uv;
		float texIndex;
		float layer;
	};

	struct SpriteQuad
	{
		Sprite vertices[4];
		float layer;
	};

	struct RendererData
	{
		uint32_t va, vb, ib;

		uint32_t whiteTex;
		uint32_t whiteTexSlot;

		uint32_t indexCount;

		Sprite* SpriteBuffer = nullptr;
		Sprite* SpriteBufferPtr = nullptr;

		uint32_t textureSlots[MaxTextures];
		uint32_t textureSlotIndex = 1;

		std::vector<SpriteQuad> allSprites;
	};

	static RendererData s_Data;

	int SpriteBatchRenderer::DrawCount = 0;
	int SpriteBatchRenderer::SpriteCount = 0;
	int SpriteBatchRenderer::TextCount = 0;
	int SpriteBatchRenderer::SpritesheetCount = 0;
	int SpriteBatchRenderer::IndicesCount = 0;
	int SpriteBatchRenderer::VerticesCount = 0;

	void SpriteBatchRenderer::Init()
	{
		s_Data.SpriteBuffer = new Sprite[MaxVertexCount];

		glGenVertexArrays(1, &s_Data.va);
		glBindVertexArray(s_Data.va);

		glGenBuffers(1, &s_Data.vb);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.vb);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Sprite), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, color));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, uv));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, texIndex));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, layer));

		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;

		for (int i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		glGenBuffers(1, &s_Data.ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenTextures(1, &s_Data.whiteTex);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, s_Data.whiteTex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		uint32_t color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		glBindTexture(GL_TEXTURE_2D, 0);

		s_Data.textureSlots[0] = s_Data.whiteTex;
		for (size_t i = 1; i < MaxTextures; i++)
		{
			s_Data.textureSlots[i] = 0;
		}
	}

	void SpriteBatchRenderer::Shutdown()
	{
		glDeleteVertexArrays(1, &s_Data.va);
		glDeleteBuffers(1, &s_Data.vb);
		glDeleteBuffers(1, &s_Data.ib);

		glDeleteTextures(1, &s_Data.whiteTex);

		delete[] s_Data.SpriteBuffer;
	}

	void SpriteBatchRenderer::Begin()
	{
		s_Data.SpriteBufferPtr = s_Data.SpriteBuffer;
	}

	void SpriteBatchRenderer::End()
	{
		std::sort(s_Data.allSprites.begin(), s_Data.allSprites.end(),
			[](const SpriteQuad& a, const SpriteQuad& b) {
				return a.layer < b.layer;
			});

		s_Data.SpriteBufferPtr = s_Data.SpriteBuffer;

		for (const auto& quad : s_Data.allSprites)
		{
			for (int i = 0; i < 4; i++)
			{
				*s_Data.SpriteBufferPtr = quad.vertices[i];
				s_Data.SpriteBufferPtr++;
			}
		}

		GLsizeiptr size = (uint8_t*)s_Data.SpriteBufferPtr - (uint8_t*)s_Data.SpriteBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.vb);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.SpriteBuffer);
	}

	void SpriteBatchRenderer::Flush()
	{
		for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, s_Data.textureSlots[i]);
		}

		glBindVertexArray(s_Data.va);

		glDrawElements(GL_TRIANGLES, s_Data.indexCount, GL_UNSIGNED_INT, nullptr);

		DrawCount++;

		s_Data.indexCount = 0;
		s_Data.textureSlotIndex = 1;
		s_Data.allSprites.clear();
	}

	void SpriteBatchRenderer::DrawQuad(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, Color color)
	{
		if (s_Data.indexCount >= MaxIndexCount)
		{
			End();
			Flush();
			Begin();
		}

		float textureIndex = 0.0f; // نفس الأبيض الافتراضي

		SpriteQuad quad;
		quad.layer = 0;

		// مركز السبرايت
		float cx = position.x + scale.x * 0.5f;
		float cy = position.y + scale.y * 0.5f;

		// 4 رؤوس قبل التدوير (إزاحات من المركز)
		zMath::Vector2 corners[4] = {
			{ -scale.x * 0.5f, -scale.y * 0.5f }, // أسفل يسار
			{  scale.x * 0.5f, -scale.y * 0.5f }, // أسفل يمين
			{  scale.x * 0.5f,  scale.y * 0.5f }, // أعلى يمين
			{ -scale.x * 0.5f,  scale.y * 0.5f }  // أعلى يسار
		};

		float cosTheta = cos(zMath::Math::Radians(rotation));
		float sinTheta = sin(zMath::Math::Radians(rotation));

		for (int i = 0; i < 4; i++)
		{
			float rotatedX = corners[i].x * cosTheta - corners[i].y * sinTheta;
			float rotatedY = corners[i].x * sinTheta + corners[i].y * cosTheta;

			//s_Data.SpriteBufferPtr->position = { cx + rotatedX, cy + rotatedY };

			//// إحداثيات الـ UV ثابتة
			//switch (i)
			//{
			//case 0: s_Data.SpriteBufferPtr->uv = { 0.0f, 0.0f }; break;
			//case 1: s_Data.SpriteBufferPtr->uv = { 1.0f, 0.0f }; break;
			//case 2: s_Data.SpriteBufferPtr->uv = { 1.0f, 1.0f }; break;
			//case 3: s_Data.SpriteBufferPtr->uv = { 0.0f, 1.0f }; break;
			//}

			//s_Data.SpriteBufferPtr->color = color;
			//s_Data.SpriteBufferPtr->texIndex = textureIndex;
			//s_Data.SpriteBufferPtr++;

			quad.vertices[i].position = { cx + rotatedX, cy + rotatedY };
			switch (i)
			{
			case 0: quad.vertices[i].uv = { 0.0f, 0.0f }; break;
			case 1: quad.vertices[i].uv = { 1.0f, 0.0f }; break;
			case 2: quad.vertices[i].uv = { 1.0f, 1.0f }; break;
			case 3: quad.vertices[i].uv = { 0.0f, 1.0f }; break;
			}
			quad.vertices[i].color = color;
			quad.vertices[i].texIndex = textureIndex;

			VerticesCount++;
		}

		s_Data.allSprites.push_back(quad);
		s_Data.indexCount += 6;
		SpriteCount++;
		IndicesCount += 6;
	}

	void SpriteBatchRenderer::DrawText(Font* font, const std::string& text, const zMath::Vector2& position, const zMath::Vector2& scale, float size, float padding, float wrap, Color color)
	{
		if (!font || text.empty())
			return;

		if (s_Data.indexCount >= MaxIndexCount || s_Data.textureSlotIndex > MaxTextures - 1)
		{
			End();
			Flush();
			Begin();
		}

		// الحصول على Texture Index للخط
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++)
		{
			if (s_Data.textureSlots[i] == font->GetID())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f && s_Data.textureSlotIndex < MaxTextures)
		{
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex++] = font->GetID();
		}

		zMath::Vector2 pos = position; // top-left origin
		float lineHeight = font->GetLineHeight() * (size / font->GetSize());
		float scaledPadding = padding * (size / font->GetSize());

		for (char c : text)
		{
			auto glyph = font->GetGlyph(c, pos);
			float scaleFactor = size / font->GetSize();

			// موقع الحرف بالنسبة لـ top-left
			float x = pos.x;
			float y = pos.y - (glyph.bbmax.y - glyph.bbmin.y) * scaleFactor;

			float w = (glyph.bbmax.x - glyph.bbmin.x) * scaleFactor;
			float h = (glyph.bbmax.y - glyph.bbmin.y) * scaleFactor;

			// الالتفاف wrap
			if (x + w > position.x + scale.x)
			{
				pos.x = position.x;
				pos.y += lineHeight + scaledPadding;
				x = pos.x;
				y = pos.y - (glyph.bbmax.y - glyph.bbmin.y) * scaleFactor;
			}

			// التوقف إذا تجاوزنا الحد العمودي
			if (y > position.y + scale.y)
				break;

			SpriteQuad quad{};
			quad.layer = 0;

			// إعداد إحداثيات الحرف
			quad.vertices[0].position = { x,        y };     // bottom-left
			quad.vertices[1].position = { x + w,    y };     // bottom-right
			quad.vertices[2].position = { x + w,    y + h }; // top-right
			quad.vertices[3].position = { x,        y + h }; // top-left

			// إعداد الـ UV
			quad.vertices[0].uv = glyph.uvmin;
			quad.vertices[1].uv = { glyph.uvmax.x, glyph.uvmin.y };
			quad.vertices[2].uv = glyph.uvmax;
			quad.vertices[3].uv = { glyph.uvmin.x, glyph.uvmax.y };

			for (int i = 0; i < 4; i++)
			{
				quad.vertices[i].color = color;
				quad.vertices[i].texIndex = textureIndex;
			}

			s_Data.allSprites.push_back(quad);

			s_Data.indexCount += 6;
			VerticesCount += 4;
			IndicesCount += 6;

			// تحريك x باستخدام -advance + padding لتصحيح المسافات
			pos.x += (glyph.advance * scaleFactor) + scaledPadding;
			pos.x -= lineHeight;
		}

		TextCount++;
	}

	void SpriteBatchRenderer::DrawSprite(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, bool flipX, bool flipY, int layer, Color color)
	{
		if (s_Data.indexCount >= MaxIndexCount || s_Data.textureSlotIndex > 31)
		{
			End();
			Flush();
			Begin();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++)
		{
			if (s_Data.textureSlots[i] == textureID)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = textureID;
			s_Data.textureSlotIndex++;
		}

		SpriteQuad quad;
		quad.layer = (float)layer;

		float cx = position.x + scale.x * 0.5f;
		float cy = position.y + scale.y * 0.5f;

		zMath::Vector2 corners[4] = {
			{ -scale.x * 0.5f, -scale.y * 0.5f },
			{  scale.x * 0.5f, -scale.y * 0.5f },
			{  scale.x * 0.5f,  scale.y * 0.5f },
			{ -scale.x * 0.5f,  scale.y * 0.5f }
		};

		zMath::Vector2 uvs[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		// لو نريد flip نعكس الإحداثيات المناسبة
		if (flipX)
		{
			// swap left/right
			std::swap(uvs[0], uvs[1]);
			std::swap(uvs[3], uvs[2]);
		}
		if (flipY)
		{
			// swap bottom/top
			std::swap(uvs[0], uvs[3]);
			std::swap(uvs[1], uvs[2]);
		}

		float cosTheta = cos(zMath::Math::Radians(rotation));
		float sinTheta = sin(zMath::Math::Radians(rotation));

		for (int i = 0; i < 4; i++)
		{
			float rotatedX = corners[i].x * cosTheta - corners[i].y * sinTheta;
			float rotatedY = corners[i].x * sinTheta + corners[i].y * cosTheta;

			quad.vertices[i].position = { cx + rotatedX, cy + rotatedY };
			quad.vertices[i].uv = uvs[i];
			quad.vertices[i].color = color;
			quad.vertices[i].texIndex = textureIndex;
			quad.vertices[i].layer = (float)layer;
		}

		s_Data.allSprites.push_back(quad);
		s_Data.indexCount += 6;
		SpriteCount++;
		IndicesCount += 6;
	}

	void SpriteBatchRenderer::DrawSpritesheet(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, bool flipX, bool flipY, int frameX, int frameY, int sheetCols, int sheetRows, int layer, Color color)
	{
		if (s_Data.indexCount >= MaxIndexCount || s_Data.textureSlotIndex > 31)
		{
			End();
			Flush();
			Begin();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++)
		{
			if (s_Data.textureSlots[i] == textureID)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = textureID;
			s_Data.textureSlotIndex++;
		}

		// احسب UV
		float uStep = 1.0f / (float)sheetCols;
		float vStep = 1.0f / (float)sheetRows;

		float u0 = frameX * uStep;
		float v0 = frameY * vStep;
		float u1 = u0 + uStep;
		float v1 = v0 + vStep;

		// flip Y for OpenGL
		float vBottom = 1.0f - v1;
		float vTop = 1.0f - v0;

		zMath::Vector2 uvMin = { u0, vBottom };
		zMath::Vector2 uvMax = { u1, vTop };

		zMath::Vector2 uvs[4] = {
		{ uvMin.x, uvMin.y }, // bottom-left
		{ uvMax.x, uvMin.y }, // bottom-right
		{ uvMax.x, uvMax.y }, // top-right
		{ uvMin.x, uvMax.y }  // top-left
		};

		if (flipX)
		{
			std::swap(uvs[0], uvs[1]);
			std::swap(uvs[3], uvs[2]);
		}
		if (flipY)
		{
			std::swap(uvs[0], uvs[3]);
			std::swap(uvs[1], uvs[2]);
		}

		SpriteQuad quad;
		quad.layer = (float)layer;

		// مركز السبرايت
		float cx = position.x + scale.x * 0.5f;
		float cy = position.y + scale.y * 0.5f;

		zMath::Vector2 corners[4] = {
			{ -scale.x * 0.5f, -scale.y * 0.5f },
			{  scale.x * 0.5f, -scale.y * 0.5f },
			{  scale.x * 0.5f,  scale.y * 0.5f },
			{ -scale.x * 0.5f,  scale.y * 0.5f }
		};

		float cosTheta = cos(zMath::Math::Radians(rotation));
		float sinTheta = sin(zMath::Math::Radians(rotation));

		for (int i = 0; i < 4; i++)
		{
			float rotatedX = corners[i].x * cosTheta - corners[i].y * sinTheta;
			float rotatedY = corners[i].x * sinTheta + corners[i].y * cosTheta;

			//s_Data.SpriteBufferPtr->position = { cx + rotatedX, cy + rotatedY };

			//switch (i)
			//{
			//case 0: s_Data.SpriteBufferPtr->uv = { uvMin.x, uvMin.y }; break; // أسفل يسار
			//case 1: s_Data.SpriteBufferPtr->uv = { uvMax.x, uvMin.y }; break; // أسفل يمين
			//case 2: s_Data.SpriteBufferPtr->uv = { uvMax.x, uvMax.y }; break; // أعلى يمين
			//case 3: s_Data.SpriteBufferPtr->uv = { uvMin.x, uvMax.y }; break; // أعلى يسار
			//}

			//s_Data.SpriteBufferPtr->color = color;
			//s_Data.SpriteBufferPtr->texIndex = textureIndex;
			//s_Data.SpriteBufferPtr->layer = layer;
			//s_Data.SpriteBufferPtr++;

			quad.vertices[i].position = { cx + rotatedX, cy + rotatedY };

			switch (i)
			{
				case 0: quad.vertices[i].uv = { uvMin.x, uvMin.y }; break; // أسفل يسار
				case 1: quad.vertices[i].uv = { uvMax.x, uvMin.y }; break; // أسفل يمين
				case 2: quad.vertices[i].uv = { uvMax.x, uvMax.y }; break; // أعلى يمين
				case 3: quad.vertices[i].uv = { uvMin.x, uvMax.y }; break; // أعلى يسار
			}
			quad.vertices[i].color = color;
			quad.vertices[i].texIndex = textureIndex;
			quad.vertices[i].layer = (float)layer;

			VerticesCount++;
		}

		s_Data.allSprites.push_back(quad);
		s_Data.indexCount += 6;
		SpritesheetCount++;
		IndicesCount += 6;
	}

	void SpriteBatchRenderer::DrawSpritePro(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, bool flipX, bool flipY, const zMath::Vector2& uv0, const zMath::Vector2& uv1, int layer, Color color)
	{
		if (s_Data.indexCount >= MaxIndexCount || s_Data.textureSlotIndex > 31)
		{
			End();
			Flush();
			Begin();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++)
		{
			if (s_Data.textureSlots[i] == textureID)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = textureID;
			s_Data.textureSlotIndex++;
		}

		SpriteQuad quad;
		quad.layer = (float)layer;

		// مركز السبرايت
		float cx = position.x + scale.x * 0.5f;
		float cy = position.y + scale.y * 0.5f;

		// 4 رؤوس قبل التدوير (نحسب الإزاحة من المركز)
		zMath::Vector2 corners[4] = {
			{ -scale.x * 0.5f, -scale.y * 0.5f }, // أسفل يسار
			{  scale.x * 0.5f, -scale.y * 0.5f }, // أسفل يمين
			{  scale.x * 0.5f,  scale.y * 0.5f }, // أعلى يمين
			{ -scale.x * 0.5f,  scale.y * 0.5f }  // أعلى يسار
		};

		float cosTheta = cos(zMath::Math::Radians(rotation));
		float sinTheta = sin(zMath::Math::Radians(rotation));

		// --- إحداثيات الـ UV حسب uv0/uv1 ---
		zMath::Vector2 uvs[4] = {
			{ uv0.x, uv0.y },
			{ uv1.x, uv0.y },
			{ uv1.x, uv1.y },
			{ uv0.x, uv1.y }
		};

		if (flipX)
		{
			std::swap(uvs[0], uvs[1]);
			std::swap(uvs[3], uvs[2]);
		}
		if (flipY)
		{
			std::swap(uvs[0], uvs[3]);
			std::swap(uvs[1], uvs[2]);
		}

		for (int i = 0; i < 4; i++)
		{
			float rotatedX = corners[i].x * cosTheta - corners[i].y * sinTheta;
			float rotatedY = corners[i].x * sinTheta + corners[i].y * cosTheta;

			/*s_Data.SpriteBufferPtr->position = { cx + rotatedX, cy + rotatedY };
			s_Data.SpriteBufferPtr->uv = uvs[i];
			s_Data.SpriteBufferPtr->color = color;
			s_Data.SpriteBufferPtr->texIndex = textureIndex;
			s_Data.SpriteBufferPtr->layer = layer;
			s_Data.SpriteBufferPtr++;*/

			quad.vertices[i].position = { cx + rotatedX, cy + rotatedY };
			quad.vertices[i].uv = uvs[i];
			quad.vertices[i].color = color;
			quad.vertices[i].texIndex = textureIndex;
			quad.vertices[i].layer = (float)layer;

			VerticesCount++;
		}

		s_Data.allSprites.push_back(quad);
		s_Data.indexCount += 6;
		SpriteCount++;
		IndicesCount += 6;
	}

	void SpriteBatchRenderer::RestStats()
	{
		DrawCount = 0;
		SpriteCount = 0;
		TextCount = 0;
		SpritesheetCount = 0;
		IndicesCount = 0;
		VerticesCount = 0;
	}

}