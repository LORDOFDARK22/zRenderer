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
	};

	class Font
	{
	private:
		uint32_t ID;
		std::string fontPath;
		int width, height;
		float size;
		void* data;

	public:
		Font(const std::string& fontPath, int width = 256, int height = 256, float size = 14);
		~Font();

		const uint32_t GetID() const { return ID; }

		inline float GetSize() { return size; }

		FontGlyph GetGlyph(char c, zMath::Vector2& position);

	};
}