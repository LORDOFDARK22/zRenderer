#pragma once
#include <iostream>
#include "Color.h"
#include <vector>

namespace zRender
{
	enum TextureType
	{
		Blended,
		Pixel,
		FRAMEBUFFER
	};

	class Texture
	{
	private:
		uint32_t ID;
		int width, height, numComponents;

	public:
		Texture();
		Texture(TextureType type, int width, int height);
		Texture(const std::string& texturePath, TextureType type = TextureType::Blended, bool flipVertically = false, bool repeated = false);
		Texture(const unsigned char* imageData, int length, TextureType type = TextureType::Blended, bool flipVertically = false, bool repeated = false);
		~Texture();

		static std::vector<Color> GetPixelsFromImage(const std::string& texturePath);

		inline uint32_t GetID() const { return ID ? ID : 0; }

		inline const int GetWidth() const { return width; }
		inline const int GetHeight() const { return height; }

		void Bind();
		void Unbind();
	};
}