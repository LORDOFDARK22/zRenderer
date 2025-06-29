#include "LinesRender.h"
#include <glad/glad.h>

namespace EWorld
{
	LinesRender::LinesRender()
	{
		glGenVertexArrays(1, &vertexArray);
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &colorBuffer);

		//glLineWidth(5);
	}
	LinesRender::~LinesRender()
	{
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &colorBuffer);
	}
	void LinesRender::AddLine(const glm::vec2& start, const glm::vec2& end, const Color& startColor, const Color& endColor)
	{
		vertices.push_back(start);
		vertices.push_back(end);

		colors.push_back(startColor);
		colors.push_back(endColor);

		UpdateLinesOnGpu();
	}
	void LinesRender::UpdateLinesOnGpu()
	{
		glBindVertexArray(vertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);

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
	void LinesRender::Render(Shader* shader, Camera& camera)
	{
		shader->Enable();
		shader->SetUniformMat4("projection", camera.GetProjection());

		glBindVertexArray(vertexArray);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glDrawArrays(GL_LINES, 0, vertices.size());

		glBindVertexArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		shader->Disable();
	}
}
