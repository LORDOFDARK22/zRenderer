#include "PixelRender.h"
#include <glad/glad.h>

namespace EWorld
{
	void PixelRender::UpdatePixelsOnGPU()
	{
		glBindVertexArray(vertexArray);
		//Vertices
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

		//Colors
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Color), colors.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Color), 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	PixelRender::PixelRender()
	{
		glGenVertexArrays(1, &vertexArray);
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &colorBuffer);

		//glPointSize(5);
	}
	PixelRender::~PixelRender()
	{
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
	}
	void PixelRender::AddPixel(const glm::vec2& pos, const Color& color)
	{
		points.push_back(pos);
		colors.push_back(color);

		//UpdateListOnGPU();
	}
	void PixelRender::Render(Shader* shader, Camera& camera)
	{
		shader->Enable();
		shader->SetUniformMat4("projection", camera.GetProjection());

		glBindVertexArray(vertexArray);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glDrawArrays(GL_POINTS, 0, points.size());

		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		shader->Disable();
	}
}
