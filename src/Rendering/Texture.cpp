#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image.h"
#include <glad/glad.h>

namespace EWorld
{
	Texture::Texture(const char* filepath)
	{
		data = stbi_load(filepath, &width, &height, &numComponents, STBI_rgb_alpha);

		if (!data)
		{
			std::cout << "Failed to Load Image [" << filepath << "]\n";
			stbi_image_free(data);
			return;
		}
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Texture::~Texture()
	{
		stbi_image_free(data);
		glDeleteTextures(1, &textureID);

		delete this;
	}
	void Texture::Enable()
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	void Texture::Disable()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}