#include "TrianglesRender.h"

namespace zRender
{
	TrianglesRender::TrianglesRender() : VAO{}, VBO{}, triangles{}
	{
	}
	TrianglesRender::~TrianglesRender()
	{
		VAO.Delete();
		VBO.Delete();
	}
	void TrianglesRender::Begin()
	{
		triangles.clear();
	}
	void TrianglesRender::End()
	{
		VAO.Bind();

		VBO.Data(triangles, true);

		VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, 6 * sizeof(float), 0);
		VAO.LinkAttrib(VBO, 1, 4, GL_FLOAT, 6 * sizeof(float), (void*)(2 * sizeof(float)));

		VAO.Unbind();
		VBO.Unbind();
	}
	void TrianglesRender::DrawTriangle(const zMath::Vector2& v1, const zMath::Vector2& v2, const zMath::Vector2& v3, Color color)
	{
		triangles.push_back(v1.x);
		triangles.push_back(v1.y);

		triangles.push_back(color.r);
		triangles.push_back(color.g);
		triangles.push_back(color.b);
		triangles.push_back(color.a);

		triangles.push_back(v2.x);
		triangles.push_back(v2.y);

		triangles.push_back(color.r);
		triangles.push_back(color.g);
		triangles.push_back(color.b);
		triangles.push_back(color.a);

		triangles.push_back(v3.x);
		triangles.push_back(v3.y);

		triangles.push_back(color.r);
		triangles.push_back(color.g);
		triangles.push_back(color.b);
		triangles.push_back(color.a);
	}
	void TrianglesRender::Render(Shader* shader, Camera2D& camera)
	{
		shader->Bind();

		shader->SetUniformMatrix4x4("projection", camera.GetProjection());
		shader->SetUniformMatrix4x4("view", camera.GetView());

		VAO.Bind();

		glDrawArrays(GL_TRIANGLES, 0, triangles.size());

		VAO.Unbind();

		shader->Unbind();
	}
}
