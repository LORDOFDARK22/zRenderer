#include "CirclesRender.h"

#include <zMath/Math.h>

namespace zRender
{
	CirclesRender::CirclesRender(float lineWidth) : VAO{}, VBO{}, IVBO{}, circles{}, line{true}
	{
		glLineWidth(lineWidth);
	}
	CirclesRender::CirclesRender() : VAO{}, VBO{}, IVBO{}, circles{}, line{false}
	{
	}
	CirclesRender::~CirclesRender()
	{
		VAO.Delete();
		VBO.Delete();
		IVBO.Delete();
	}
	void CirclesRender::Begin()
	{
		circles.clear();
	}
	void CirclesRender::End()
	{
		int segments = 25;

		std::vector<float> circleVerts;
		circleVerts.push_back(0.0f); // المركز
		circleVerts.push_back(0.0f);

		for (int i = 0; i <= segments; i++) {
			float theta = 2.0f * zMath::Math::PI * float(i) / float(segments);
			circleVerts.push_back(cosf(theta));
			circleVerts.push_back(sinf(theta));
		}
		int circleVertexCount = segments + 2;

		VAO.Bind();

		VBO.Bind();
		VBO.Data(circleVerts, false);

		VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, 2 * sizeof(float), 0);

		IVBO.Bind();
		IVBO.Data(circles, true);

		VAO.LinkAttrib(IVBO, 1, 2, GL_FLOAT, 7 * sizeof(float), 0);
		glVertexAttribDivisor(1, 1);

		VAO.LinkAttrib(IVBO, 2, 1, GL_FLOAT, 7 * sizeof(float), (void*)(2 * sizeof(float)));
		glVertexAttribDivisor(2, 1);

		VAO.LinkAttrib(IVBO, 3, 4, GL_FLOAT, 7 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(3, 1);

		VAO.Unbind();
		VBO.Unbind();
		IVBO.Unbind();
	}
	void CirclesRender::DrawCircle(const zMath::Vector2& position, float radius, Color color)
	{
		circles.push_back(position.x);
		circles.push_back(position.y);
		circles.push_back(radius);

		circles.push_back(color.r);
		circles.push_back(color.g);
		circles.push_back(color.b);
		circles.push_back(color.a);
	}
	void CirclesRender::Render(Shader* shader, Camera2D& camera)
	{
		shader->Bind();

		shader->SetUniformMatrix4x4("projection", camera.GetProjection());
		shader->SetUniformMatrix4x4("view", camera.GetView());

		VAO.Bind();

		if (line)
			glDrawArraysInstanced(GL_LINE_LOOP, 0, 27, circles.size());
		else
			glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 27, circles.size());

		VAO.Unbind();

		shader->Unbind();
	}
}
