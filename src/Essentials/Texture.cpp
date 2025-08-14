#include "Texture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image.h"

#include <string>

namespace zRender
{
	Texture::Texture()
	{
		glGenTextures(1, &ID);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		uint32_t color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Texture::Texture(const std::string& texturePath, bool flipVertically, TextureType type)
	{
		stbi_set_flip_vertically_on_load(flipVertically);
		unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numComponents, 0);

		if (!data)
		{
			std::cout << "Failed to Load Image [" << texturePath << "]\n";
			stbi_image_free(data);
			return;
		}

		glGenTextures(1, &ID);
		//glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, ID);

		if (type == TextureType::Pixel)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if (type == TextureType::Blended || type == TextureType::None)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if (type == TextureType::Repeated)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (numComponents == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (numComponents == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (numComponents == 1)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			throw std::invalid_argument("Automatic Texture Type rec failed!");
		}

		//glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		if (ID)
			glDeleteTextures(1, &ID);
	}
	std::vector<Color> Texture::GetPixelsFromImage(const std::string& texturePath)
	{
		std::vector<Color> pixels;

		int width, height, numComponents;

		unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numComponents, 0);

		if (!data)
		{
			std::cout << "Failed to Load Image [" << texturePath << "]\n";
			stbi_image_free(data);
			return std::vector<Color>();
		}

		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				int index = (y * width + x) * numComponents;
				unsigned char r = data[index + 0];
				unsigned char g = data[index + 1];
				unsigned char b = data[index + 2];
				unsigned char a = numComponents == 4 ? data[index + 3] : 255;

				//std::cout << "Pixel at (0,0): R=" << (int)r << " G=" << (int)g << " B=" << (int)b << " A=" << (int)a << "\n";

				pixels.push_back(Color{ (float)r, (float)g, (float)b, (float)a });
			}
		}

		return pixels;
	}
	void Texture::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}
	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
