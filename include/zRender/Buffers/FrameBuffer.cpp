#include "FrameBuffer.h"

namespace zRender
{
	void FrameBuffer::CleanUp()
	{
		glDeleteFramebuffers(1, &FBO);
		if (useRbo)
			glDeleteRenderbuffers(1, &RBO);
	}
	FrameBuffer::FrameBuffer() : FrameBuffer(800, 600, false)
	{
	}
	FrameBuffer::FrameBuffer(int width, int height, bool useRbo) : width{width}, height{height}, useRbo{useRbo}, shouldResize{false}, FBO{0}, RBO{0}, texture{nullptr}
	{
		texture = std::make_shared<Texture>(TextureType::FRAMEBUFFER, width, height);
		if (!texture || !Initialize())
		{
			std::cout << "Failed to create FrameBuffer!" << std::endl;
			return;
		}
	}
	FrameBuffer::~FrameBuffer()
	{
		CleanUp();
	}
	bool FrameBuffer::Initialize()
	{
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0);

		if (useRbo)
		{
			glGenRenderbuffers(1, &RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, RBO);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::string error = std::to_string(glGetError());
			std::cout << "failed to create an OpenGL FrameBuffer : " << error << std::endl;
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}
	void FrameBuffer::Resize(int width, int height)
	{
		this->width = width;
		this->height = height;
		shouldResize = true;
	}
	void FrameBuffer::CheckResize()
	{
		if (!shouldResize)
			return;

		CleanUp();
		texture.reset();

		texture = std::move(std::make_shared<Texture>(TextureType::FRAMEBUFFER, width, height));
		if (!texture)
		{
			std::cout << "New Texture cannot at nullptr!" << std::endl;
			return;
		}

		Initialize();
		shouldResize = false;
	}
	void FrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}
	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
