#include "Sprite.h"
#include <glad/glad.h>

namespace EWorld
{
	Sprite::Sprite(const char* imagePath, const glm::vec2& position, const glm::vec2& scale, float angle, const Color& color) : position{ position }, scale{ scale }, angle{ angle }, color { color }
	{
		texture = new Texture{ imagePath };

		std::vector<glm::vec2> vertices =
		{
			{-1.0f, -1.0f},		{1.0f, 1.0f},
			{1.0f, -1.0f},		{1.0f, 0.0f},
			{1.0f, 1.0f},		{0.0f, 0.0f},
			{-1.0f, 1.0f},		{0.0f, 1.0f}
		};

		std::vector<int> indices =
		{
			//FirstTriangle
			0, 1, 3,
			//TwoTriangle
			1, 2, 3
		};

		glGenVertexArrays(1, &vertexArray);
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &ebo);
		//glGenBuffers(1, &colorBuffer);
		
		glBindVertexArray(vertexArray);

		//Position
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);

		//pos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec2), 0);

		//uv
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec2), (void*)(sizeof(glm::vec2)));

		//Colors
		//glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(Color), &color, GL_DYNAMIC_DRAW);

		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Color), 0);

		//Rest
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	Sprite::~Sprite()
	{
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &ebo);
	}
	void Sprite::Render(Shader* shader, Camera& camera)
	{
		shader->Enable();

		shader->SetUniformMat4("projection", camera.GetProjection());

		//shader->SetUniform1i("ourTexture", 0);

		glm::mat4 model = GetModelMatrix();
		shader->SetUniformMat4("model", model);

		texture->Enable();

		glBindVertexArray(vertexArray);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		texture->Disable();

		glBindVertexArray(0);

		shader->Disable();
	}
	glm::mat4 Sprite::GetModelMatrix()
	{
		glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ position, 0.0f });
		glm::mat4 scl = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ scale, 1.0f });
		glm::mat4 rotation = glm::rotate(glm::mat4{ 1.0f }, angle, glm::vec3{ 0.0f, 0.0f, 1.0f });

		return translation * scl * rotation;
	}
}