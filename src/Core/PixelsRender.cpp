#include "PixelsRender.h"

namespace zRender
{
	PixelsRender::PixelsRender() : PixelsRender(1)
	{
	}
	PixelsRender::PixelsRender(float pixelSize) : VAO{}, VBO{}
	{
		glPointSize(pixelSize);
	}
	PixelsRender::~PixelsRender()
	{
		VAO.Delete();
		VBO.Delete();
	}
	void PixelsRender::Begin()
	{
		pixels.clear();
	}
	void PixelsRender::End()
	{
		VAO.Bind();

		VBO.Data(pixels, true);

		VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, 6 * sizeof(float), 0);
		VAO.LinkAttrib(VBO, 1, 4, GL_FLOAT, 6 * sizeof(float), (void*)(2 * sizeof(float)));

		VAO.Unbind();
		VBO.Unbind();
	}
	void PixelsRender::DrawPixel(const zMath::Vector2& position, Color color)
	{
		pixels.push_back(position.x);
		pixels.push_back(position.y);

		pixels.push_back(color.r);
		pixels.push_back(color.g);
		pixels.push_back(color.b);
		pixels.push_back(color.a);
	}
	void PixelsRender::Render(Shader* shader, Camera2D& camera)
	{
		shader->Bind();
		
		shader->SetUniformMatrix4x4("projection", camera.GetProjection());

		VAO.Bind();

		glDrawArrays(GL_POINTS, 0, pixels.size());

		VAO.Unbind();

		shader->Unbind();
	}
}
