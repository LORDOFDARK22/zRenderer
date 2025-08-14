#include "LinesRender.h"

namespace zRender
{
	LinesRender::LinesRender() : LinesRender(1)
	{
	}
	LinesRender::LinesRender(float lineWidth) : VAO{}, VBO{}
	{
		glLineWidth(lineWidth);
	}
	LinesRender::~LinesRender()
	{
		VAO.Delete();
		VBO.Delete();
	}
	void LinesRender::Begin()
	{
		lines.clear();
	}
	void LinesRender::End()
	{
		VAO.Bind();

		VBO.Data(lines, true);

		VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, 6 * sizeof(float), 0);
		VAO.LinkAttrib(VBO, 1, 4, GL_FLOAT, 6 * sizeof(float), (void*)(2 * sizeof(float)));

		VAO.Unbind();
		VBO.Unbind();
	}
	void LinesRender::DrawLine(const zMath::Vector2& start, const zMath::Vector2& end, Color color)
	{
		lines.push_back(start.x);
		lines.push_back(start.y);

		lines.push_back(color.r);
		lines.push_back(color.g);
		lines.push_back(color.b);
		lines.push_back(color.a);

		lines.push_back(end.x);
		lines.push_back(end.y);

		lines.push_back(color.r);
		lines.push_back(color.g);
		lines.push_back(color.b);
		lines.push_back(color.a);
	}
	void LinesRender::Render(Shader* shader, Camera2D& camera)
	{
		shader->Bind();

		shader->SetUniformMatrix4x4("projection", camera.GetProjection());
		shader->SetUniformMatrix4x4("view", camera.GetView());

		VAO.Bind();

		glDrawArrays(GL_LINES, 0, lines.size());

		VAO.Unbind();

		shader->Unbind();
	}
}