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
		Repeated,
		FRAMEBUFFER,
		None
	};

	class Texture
	{
	private:
		uint32_t ID;
		int width, height, numComponents;

	public:
		Texture();
		Texture(const unsigned char* imageData, int length, bool flipVertically, TextureType type = TextureType::Blended);
		Texture(TextureType type, int width, int height);
		Texture(const std::string& texturePath, bool flipVertically, TextureType type = TextureType::Blended);
		~Texture();

		static std::vector<Color> GetPixelsFromImage(const std::string& texturePath);

		inline uint32_t GetID() const { return ID ? ID : 0; }

		inline const int GetWidth() const { return width; }
		inline const int GetHeight() const { return height; }

		void Bind();
		void Unbind();
	};
}