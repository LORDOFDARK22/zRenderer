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
		None
	};
	class Texture
	{
	private:
		uint32_t ID;
		int width, height, numComponents;

	public:
		Texture();
		Texture(const std::string& texturePath, bool flipVertically, TextureType type = TextureType::Blended);
		~Texture();

		static std::vector<Color> GetPixelsFromImage(const std::string& texturePath);

		const uint32_t GetID() const { return ID; }

		void Bind();
		void Unbind();
	};
}