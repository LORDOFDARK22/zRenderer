#pragma once
#include <iostream>

namespace EWorld
{
	class Texture
	{
	private:
		uint32_t textureID;
		int width, height, numComponents;
		unsigned char* data;

	public:
		Texture(const char* filepath);
		~Texture();

		void Enable();
		void Disable();

	};
}