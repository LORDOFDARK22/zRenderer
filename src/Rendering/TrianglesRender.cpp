#include "TrianglesRender.h"
#include <glad/glad.h>

namespace EWorld
{
	TrianglesRender::TrianglesRender()
	{
		glGenVertexArrays(1, &vertexArray);
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &colorBuffer);
	}
	TrianglesRender::~TrianglesRender()
	{
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &colorBuffer);
	}
	void TrianglesRender::UpdateTrianglesOnGpu()
	{
		glBindVertexArray(vertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Color), colors.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Color), 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void TrianglesRender::AddTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3, const Color& color)
	{
		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		colors.push_back(color);
		colors.push_back(color);
		colors.push_back(color);

		//UpdateTrianglesOnGpu();
	}
	void TrianglesRender::Render(Shader* shader, Camera& camera)
	{
		shader->Enable();

		shader->SetUniformMat4("projection", camera.GetProjection());

		glBindVertexArray(vertexArray);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glBindVertexArray(0);

		shader->Disable();
	}
}
