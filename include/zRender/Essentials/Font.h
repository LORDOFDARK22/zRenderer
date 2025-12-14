#pragma once
#include <iostream>
#include <zMath/Vector2.h>
#include <vector>

namespace zRender
{
	struct FontGlyph
	{
		zMath::Vector2 bbmin;
		zMath::Vector2 bbmax;
		zMath::Vector2 uvmin;
		zMath::Vector2 uvmax;
		zMath::Vector2 bearing;
		float advance;
	};

	class Font
	{
	private:
		uint32_t ID = 0;
		std::string fontPath;
		int width = 0, height = 0;
		float size = 14.0f;
		void* data = nullptr;

		// ارتفاع السطر الحقيقي
		float lineHeight = 0.0f;

	public:
		Font();
		Font(const unsigned char* fontData, int width = 256, int height = 256, float size = 14);
		Font(const std::string& fontPath, int width = 256, int height = 256, float size = 14);
		~Font();

		inline uint32_t GetID() const { return ID; }

		inline float GetSize() const { return size; }

		FontGlyph GetGlyph(char c, zMath::Vector2& position);

		// —- دالة الحصول على ارتفاع السطر —-
		inline float GetLineHeight() const
		{
			return (lineHeight > 0.0f) ? lineHeight : size * 1.2f;
		}

		// —- دالة يتم استدعاؤها من داخل الـ CPP —-
		inline void SetLineHeight(float lh)
		{
			lineHeight = lh;
		}
	};
}
