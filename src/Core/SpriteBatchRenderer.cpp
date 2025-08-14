#include "SpriteBatchRenderer.h"

#include <glad/glad.h>
#include <gl/GL.h>
#include <array>

#include <zMath/Vector3.h>
#include <zMath/Math.h>

namespace zRender
{
	static const size_t MaxSpriteCount = 10000;
	static const size_t MaxVertexCount = MaxSpriteCount * 4;
	static const size_t MaxIndexCount = MaxSpriteCount * 6;
	static const size_t MaxTextures = 32;

	struct Sprite
	{
		zMath::Vector3 position;
		Color color;
		zMath::Vector2 uv;
		float texIndex;
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
	};

	static RendererData s_Data;

	int SpriteBatchRenderer::DrawCount = 0;
	int SpriteBatchRenderer::SpriteCount = 0;

	void SpriteBatchRenderer::Init()
	{
		s_Data.SpriteBuffer = new Sprite[MaxVertexCount];

		glGenVertexArrays(1, &s_Data.va);
		glBindVertexArray(s_Data.va);

		glGenBuffers(1, &s_Data.vb);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.vb);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Sprite), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, color));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, uv));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (const void*)offsetof(Sprite, texIndex));

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
		//Debug
		//glDrawElements(GL_LINE_LOOP, s_Data.indexCount, GL_UNSIGNED_INT, nullptr);
		DrawCount++;

		s_Data.indexCount = 0;
		s_Data.textureSlotIndex = 1;
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

		float cosTheta = cos(zMath::Math::radians(rotation));
		float sinTheta = sin(zMath::Math::radians(rotation));

		for (int i = 0; i < 4; i++)
		{
			float rotatedX = corners[i].x * cosTheta - corners[i].y * sinTheta;
			float rotatedY = corners[i].x * sinTheta + corners[i].y * cosTheta;

			s_Data.SpriteBufferPtr->position = { cx + rotatedX, cy + rotatedY, 0.0f };

			// إحداثيات الـ UV ثابتة
			switch (i)
			{
			case 0: s_Data.SpriteBufferPtr->uv = { 0.0f, 0.0f }; break;
			case 1: s_Data.SpriteBufferPtr->uv = { 1.0f, 0.0f }; break;
			case 2: s_Data.SpriteBufferPtr->uv = { 1.0f, 1.0f }; break;
			case 3: s_Data.SpriteBufferPtr->uv = { 0.0f, 1.0f }; break;
			}

			s_Data.SpriteBufferPtr->color = color;
			s_Data.SpriteBufferPtr->texIndex = textureIndex;
			s_Data.SpriteBufferPtr++;
		}

		s_Data.indexCount += 6;
		SpriteCount++;
	}

	void SpriteBatchRenderer::DrawText(Font* font, const std::string& text, const zMath::Vector2& position, const zMath::Vector2& scale, float size, float padding, float wrap, Color color)
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
			if (s_Data.textureSlots[i] == font->GetID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = font->GetID();
			s_Data.textureSlotIndex++;
		}

		zMath::Vector2 pos = position;
		for (auto c : text)
		{
			auto glyph = font->GetGlyph(c, pos);

			float scaleFactor = size / font->GetSize();

			glyph.bbmin *= scaleFactor;
			glyph.bbmax *= scaleFactor;

			// الالتفاف الأفقي
			if (glyph.bbmax.x > position.x + scale.x)
			{
				pos.y += font->GetSize() + wrap;
				pos.x = position.x;
			}

			// التوقف إذا طلعنا بره العمودي
			if (glyph.bbmax.y > position.y + scale.y)
			{
				return;
			}

			s_Data.SpriteBufferPtr->position = { glyph.bbmin.x, glyph.bbmin.y, 0.0f };
			s_Data.SpriteBufferPtr->color = color;
			s_Data.SpriteBufferPtr->uv = glyph.uvmin;
			s_Data.SpriteBufferPtr->texIndex = textureIndex;
			s_Data.SpriteBufferPtr++;

			s_Data.SpriteBufferPtr->position = { glyph.bbmax.x, glyph.bbmin.y, 0.0f };
			s_Data.SpriteBufferPtr->color = color;
			s_Data.SpriteBufferPtr->uv = { glyph.uvmax.x, glyph.uvmin.y };
			s_Data.SpriteBufferPtr->texIndex = textureIndex;
			s_Data.SpriteBufferPtr++;

			s_Data.SpriteBufferPtr->position = { glyph.bbmax.x, glyph.bbmax.y, 0.0f };
			s_Data.SpriteBufferPtr->color = color;
			s_Data.SpriteBufferPtr->uv = { glyph.uvmax.x, glyph.uvmax.y };
			s_Data.SpriteBufferPtr->texIndex = textureIndex;
			s_Data.SpriteBufferPtr++;

			s_Data.SpriteBufferPtr->position = { glyph.bbmin.x, glyph.bbmax.y, 0.0f };
			s_Data.SpriteBufferPtr->color = color;
			s_Data.SpriteBufferPtr->uv = { glyph.uvmin.x, glyph.uvmax.y };
			s_Data.SpriteBufferPtr->texIndex = textureIndex;
			s_Data.SpriteBufferPtr++;

			//padding
			pos.x += padding;

			s_Data.indexCount += 6;
			SpriteCount++;
		}
	}

	void SpriteBatchRenderer::DrawSprite(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, Color color)
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

		float cosTheta = cos(zMath::Math::radians(rotation));
		float sinTheta = sin(zMath::Math::radians(rotation));

		for (int i = 0; i < 4; i++)
		{
			float rotatedX = corners[i].x * cosTheta - corners[i].y * sinTheta;
			float rotatedY = corners[i].x * sinTheta + corners[i].y * cosTheta;

			s_Data.SpriteBufferPtr->position = { cx + rotatedX, cy + rotatedY, 0.0f };
        
			// نفس الالوان والإحداثيات UV المعتادة
			switch(i)
			{
				case 0: s_Data.SpriteBufferPtr->uv = { 0.0f, 0.0f }; break;
				case 1: s_Data.SpriteBufferPtr->uv = { 1.0f, 0.0f }; break;
				case 2: s_Data.SpriteBufferPtr->uv = { 1.0f, 1.0f }; break;
				case 3: s_Data.SpriteBufferPtr->uv = { 0.0f, 1.0f }; break;
			}

			s_Data.SpriteBufferPtr->color = color;
			s_Data.SpriteBufferPtr->texIndex = textureIndex;
			s_Data.SpriteBufferPtr++;
		}

		s_Data.indexCount += 6;
		SpriteCount++;
	}

	void SpriteBatchRenderer::DrawSpritesheet(const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, uint32_t textureID, int frameX, int frameY, int sheetCols, int sheetRows, Color color)
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

		zMath::Vector2 uvMin = { frameX * uStep, frameY * vStep };
		zMath::Vector2 uvMax = { uvMin.x + uStep, uvMin.y + vStep };

		// قلب الـ V axis لأن OpenGL تعتبر (0,0) تحت
		uvMin.y = 1.0f - uvMin.y - vStep;
		uvMax.y = 1.0f - uvMax.y + vStep;

		// مركز السبرايت
		float cx = position.x + scale.x * 0.5f;
		float cy = position.y + scale.y * 0.5f;

		zMath::Vector2 corners[4] = {
			{ -scale.x * 0.5f, -scale.y * 0.5f },
			{  scale.x * 0.5f, -scale.y * 0.5f },
			{  scale.x * 0.5f,  scale.y * 0.5f },
			{ -scale.x * 0.5f,  scale.y * 0.5f }
		};

		float cosTheta = cos(zMath::Math::radians(rotation));
		float sinTheta = sin(zMath::Math::radians(rotation));

		for (int i = 0; i < 4; i++)
		{
			float rotatedX = corners[i].x * cosTheta - corners[i].y * sinTheta;
			float rotatedY = corners[i].x * sinTheta + corners[i].y * cosTheta;

			s_Data.SpriteBufferPtr->position = { cx + rotatedX, cy + rotatedY, 0.0f };

			switch (i)
			{
			case 0: s_Data.SpriteBufferPtr->uv = { uvMin.x, uvMin.y }; break; // أسفل يسار
			case 1: s_Data.SpriteBufferPtr->uv = { uvMax.x, uvMin.y }; break; // أسفل يمين
			case 2: s_Data.SpriteBufferPtr->uv = { uvMax.x, uvMax.y }; break; // أعلى يمين
			case 3: s_Data.SpriteBufferPtr->uv = { uvMin.x, uvMax.y }; break; // أعلى يسار
			}

			s_Data.SpriteBufferPtr->color = color;
			s_Data.SpriteBufferPtr->texIndex = textureIndex;
			s_Data.SpriteBufferPtr++;
		}

		s_Data.indexCount += 6;
		SpriteCount++;
	}

	void SpriteBatchRenderer::RestStats()
	{
		DrawCount = 0;
		SpriteCount = 0;
	}

}