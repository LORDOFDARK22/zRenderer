#include "Triangle.h"
#include <glad/glad.h>

namespace EWorld
{
	glm::mat4 Triangle::GetModelMatrix()
	{
		glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ position, 0.0f });
		glm::mat4 scl = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ scale, 1.0f });
		glm::mat4 rotation = glm::rotate(glm::mat4{ 1.0f }, angle, glm::vec3{ 0.0f, 0.0f, 1.0f });

		return translation * scl * rotation;
	}
	Triangle::Triangle(const glm::vec2& position, const glm::vec2& scale, float angle, const Color& color) : position{position}, scale{scale}, angle{angle}, color{color}
	{
		//Fix Optimization = (3 vec2) just for all Triangles | example 10.000 triangles * 24 bytes
		std::vector<glm::vec2> triangleVertices =
		{
			{-1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f}
		};
		//Fix Optimization = (1 color) for per trianlge | example  10.000 triangles * 48
		std::vector<Color> colors =
		{
			color, color, color
		};

		glGenVertexArrays(1, &vertexArray);
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &colorBuffer);

		glBindVertexArray(vertexArray);

		//Position
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec2), triangleVertices.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

		//Colors
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Color), colors.data(), GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Color), 0);

		//Rest
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	Triangle::~Triangle()
	{
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &colorBuffer);

		delete this;
	}
	void Triangle::Render(Shader* shader, Camera& camera)
	{
		shader->Enable();

		shader->SetUniformMat4("projection", camera.GetProjection());

		glm::mat4 model = GetModelMatrix();
		shader->SetUniformMat4("model", model);

		glBindVertexArray(vertexArray);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		shader->Disable();
	}
}
