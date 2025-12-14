#pragma once
#include <glad/glad.h>
#include <memory>

#include "../Essentials/Texture.h"

namespace zRender
{
	class FrameBuffer
	{
	private:
		uint32_t FBO, RBO;
		std::shared_ptr<Texture> texture;
		int width, height;
		bool shouldResize, useRbo;

		void CleanUp();
		bool Initialize();

	public:
		FrameBuffer();
		FrameBuffer(int width, int height, bool useRbo);
		~FrameBuffer();

		void Bind();
		void Unbind();

		void Resize(int width, int height);
		void CheckResize();

		inline const uint32_t GetID() const { return FBO; }
		inline const uint32_t GetTextureID() const { return texture.get()->GetID(); }
		inline const int GetWidth() const { return width; }
		inline const int GetHeight() const { return height; }

	};
}