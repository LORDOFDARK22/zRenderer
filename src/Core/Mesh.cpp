#include "Mesh.h"

namespace zRender
{
	Mesh::Mesh(bool isStatic) : VAO{}, VBO{}, EBO{}, isStatic{isStatic}
	{
	}
	Mesh::Mesh() : VAO{}, VBO{}, EBO{}
	{
	}
	Mesh::~Mesh()
	{
		VAO.Delete();
		VBO.Delete();
		EBO.Delete();
	}
	void Mesh::Begin()
	{
		vertices.clear();
		indices.clear();
	}
	void Mesh::End()
	{
		VAO.Bind();

		VBO.Data(vertices, true);
		EBO.Data(indices, true);

		VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
		VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
		VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
		VAO.LinkAttrib(VBO, 3, 4, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));

		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	}
	void Mesh::Render(Shader* shader, Texture* texture, Camera2D& camera)
	{
		shader->Bind();

		shader->SetUniformMatrix4x4("projection", camera.GetProjection());
		
		if (!isStatic)
			shader->SetUniformMatrix4x4("view", camera.GetView());
		else
			shader->SetUniformMatrix4x4("view", zMath::Matrix4x4::identity);

		texture->Bind();

		VAO.Bind();

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		VAO.Unbind();

		texture->Unbind();

		shader->Unbind();
	}
}
