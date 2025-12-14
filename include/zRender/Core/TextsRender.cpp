#include "TextsRender.h"

#define MAX_FONTS 32
#define MAX_CHARS 1000000

namespace zRender
{
	TextsRender::TextsRender() : VAO{}, VBO{}
	{
		fonts.reserve(MAX_FONTS);
		texts.reserve(MAX_CHARS * 6 * 9); // 6 vertices × 9 floats

		VAO.Bind();
		VBO.Bind();

		VBO.Data(nullptr, MAX_CHARS * 6 * 9 * sizeof(float), true);

		VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, 9 * sizeof(float), (void*)0);//position
		VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, 9 * sizeof(float), (void*)(2 * sizeof(float)));//uv
		VAO.LinkAttrib(VBO, 2, 4, GL_FLOAT, 9 * sizeof(float), (void*)(4 * sizeof(float)));//color
		VAO.LinkAttrib(VBO, 3, 1, GL_FLOAT, 9 * sizeof(float), (void*)(8 * sizeof(float)));//indexFonts

		VBO.Unbind();
		VAO.Unbind();
	}
	TextsRender::~TextsRender()
	{
		VAO.Delete();
		VBO.Delete();
	}
	void TextsRender::Begin()
	{
		texts.clear();
		fonts.clear();
	}
	void TextsRender::End()
	{
		VAO.Bind();
		VBO.SubData(0, texts.data(), texts.size() * sizeof(float));
		VAO.Unbind();
	}
	void TextsRender::DrawText(Font* font, const std::string& text, const zMath::Vector2& position, const zMath::Vector2& scale, float size, float padding, float wrap, Color color)
	{
		// إدارة التكسشر (مثل كودك السابق)
		float texID = 0.0f;
		auto it = std::find(fonts.begin(), fonts.end(), font->GetID());
		if (it != fonts.end()) {
			texID = static_cast<float>(it - fonts.begin());
		}
		else {
			if (fonts.size() >= MAX_FONTS) {
				End();
				Render(currentShader, *currentCamera);
				Begin();
			}
			fonts.push_back(font->GetID());
			texID = static_cast<float>(fonts.size() - 1);
		}

		zMath::Vector2 pos = position;

		for (char32_t c : text)
		{
			auto glyph = font->GetGlyph(c, pos);

			float scaleFactor = size / font->GetSize();

			glyph.bbmin *= scaleFactor;
			glyph.bbmax *= scaleFactor;

			zMath::Vector2 glyphSize = glyph.bbmax - glyph.bbmin;

			// الالتفاف الأفقي
			if (glyph.bbmax.x > position.x + scale.x)
			{
				pos.y += font->GetSize() + wrap;
				pos.x = position.x;
			}

			// التوقف إذا طلعنا بره العمودي
			if (glyph.bbmax.y > position.y + scale.y)
			{
				return;
			}

			//vertex 1
			texts.push_back(glyph.bbmin.x);
			texts.push_back(glyph.bbmin.y);

			texts.push_back(glyph.uvmin.x);
			texts.push_back(glyph.uvmin.y);

			texts.push_back(color.r);
			texts.push_back(color.g);
			texts.push_back(color.b);
			texts.push_back(color.a);

			texts.push_back(texID);

			//vertex 2
			texts.push_back(glyph.bbmax.x);
			texts.push_back(glyph.bbmin.y);

			texts.push_back(glyph.uvmax.x);
			texts.push_back(glyph.uvmin.y);

			texts.push_back(color.r);
			texts.push_back(color.g);
			texts.push_back(color.b);
			texts.push_back(color.a);

			texts.push_back(texID);

			//vertex 3
			texts.push_back(glyph.bbmax.x);
			texts.push_back(glyph.bbmax.y);

			texts.push_back(glyph.uvmax.x);
			texts.push_back(glyph.uvmax.y);

			texts.push_back(color.r);
			texts.push_back(color.g);
			texts.push_back(color.b);
			texts.push_back(color.a);

			texts.push_back(texID);

			//

			//vertex 4
			texts.push_back(glyph.bbmin.x);
			texts.push_back(glyph.bbmin.y);

			texts.push_back(glyph.uvmin.x);
			texts.push_back(glyph.uvmin.y);

			texts.push_back(color.r);
			texts.push_back(color.g);
			texts.push_back(color.b);
			texts.push_back(color.a);

			texts.push_back(texID);

			//vertex 5
			texts.push_back(glyph.bbmax.x);
			texts.push_back(glyph.bbmax.y);

			texts.push_back(glyph.uvmax.x);
			texts.push_back(glyph.uvmax.y);

			texts.push_back(color.r);
			texts.push_back(color.g);
			texts.push_back(color.b);
			texts.push_back(color.a);

			texts.push_back(texID);

			//vertex 6
			texts.push_back(glyph.bbmin.x);
			texts.push_back(glyph.bbmax.y);

			texts.push_back(glyph.uvmin.x);
			texts.push_back(glyph.uvmax.y);

			texts.push_back(color.r);
			texts.push_back(color.g);
			texts.push_back(color.b);
			texts.push_back(color.a);

			texts.push_back(texID);

			//padding
			pos.x += padding;
		}
	}
	void TextsRender::Render(Shader* shader, Camera2D& camera)
	{
		currentShader = shader;
		currentCamera = &camera;

		shader->Bind();

		shader->SetUniformMatrix4x4("projection", camera.GetProjection());
		shader->SetUniformMatrix4x4("view", camera.GetView());

		for (size_t i = 0; i < fonts.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, fonts[i]);
		}

		int samplers[MAX_FONTS];
		for (int i = 0; i < MAX_FONTS; ++i) samplers[i] = i;
		shader->SetUniformIntArray("u_FontsAtlas", MAX_FONTS, samplers);

		VAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(texts.size() / 9));
		VAO.Unbind();

		shader->Unbind();
	}
}
