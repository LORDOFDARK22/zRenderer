#include "SpritesRender.h"

#include <zMath/Math.h>

#define MAX_TEXTURES 32 // يعتمد على GL_MAX_TEXTURE_IMAGE_UNITS
#define MAX_SPRITES 100000

namespace zRender
{
	SpritesRender::SpritesRender() : VAO{}, VBO{}
	{
		textures.reserve(MAX_TEXTURES);
		sprites.reserve(MAX_SPRITES * 6 * 9); // 6 vertices × 9 floats

		VAO.Bind();
		VBO.Bind();

		VBO.Data(nullptr, MAX_SPRITES * 6 * 9 * sizeof(float), true);

		VAO.LinkAttrib(VBO, 0, 2, GL_FLOAT, 9 * sizeof(float), (void*)0);
		VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, 9 * sizeof(float), (void*)(2 * sizeof(float)));
		VAO.LinkAttrib(VBO, 2, 4, GL_FLOAT, 9 * sizeof(float), (void*)(4 * sizeof(float)));
		VAO.LinkAttrib(VBO, 3, 1, GL_FLOAT, 9 * sizeof(float), (void*)(8 * sizeof(float)));

		VBO.Unbind();
		VAO.Unbind();
	}
	SpritesRender::~SpritesRender()
	{
		VAO.Delete();
		VBO.Delete();
	}
	void SpritesRender::Begin()
	{
		sprites.clear();
		textures.clear();
	}
	void SpritesRender::End()
	{
		VAO.Bind();
		VBO.SubData(0, sprites.data(), sprites.size() * sizeof(float));
		VAO.Unbind();
	}
    void SpritesRender::DrawSprite(uint32_t textureID, const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, Color color)
    {
        // إدارة التكسشر (مثل كودك السابق)
        float texID = 0.0f;
        auto it = std::find(textures.begin(), textures.end(), textureID);
        if (it != textures.end()) {
            texID = static_cast<float>(it - textures.begin());
        }
        else {
            if (textures.size() >= MAX_TEXTURES) {
                End();
                Render(currentShader, *currentCamera);
                Begin();
            }
            textures.push_back(textureID);
            texID = static_cast<float>(textures.size() - 1);
        }

        static constexpr float quad[6][2] = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
            {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
        };

        // UV لكل vertex بناءً على الإطار
        float texCoords[6][2] = {
            {0, 0}, {1, 0}, {1, 1},
            {0, 0}, {0, 1}, {1, 1}
        };

        // مركز الدوران
        float halfW = scale.x * 0.5f;
        float halfH = scale.y * 0.5f;
        float centerX = position.x + halfW;
        float centerY = position.y + halfH;

        float s = sinf(zMath::Math::radians(rotation));
        float c = cosf(zMath::Math::radians(rotation));

        for (int i = 0; i < 6; ++i) {
            // إحداثيات محلية بالنسبة للمركز
            float localX = quad[i][0] * scale.x - halfW;
            float localY = quad[i][1] * scale.y - halfH;

            // دوران
            float rx = localX * c - localY * s;
            float ry = localX * s + localY * c;

            // إحداثيات نهائية
            float finalX = centerX + rx;
            float finalY = centerY + ry;

            // إضافة البيانات للبفر
            sprites.push_back(finalX);
            sprites.push_back(finalY);

            sprites.push_back(texCoords[i][0]);
            sprites.push_back(texCoords[i][1]);

            sprites.push_back(color.r);
            sprites.push_back(color.g);
            sprites.push_back(color.b);
            sprites.push_back(color.a);

            sprites.push_back(texID);
        }
    }
    void SpritesRender::DrawSpritesheet(uint32_t textureID, int frameX, int frameY, int sheetCols, int sheetRows, const zMath::Vector2& position, const zMath::Vector2& scale, float rotation, Color color)
	{
        // إدارة التكسشر (مثل كودك السابق)
        float texID = 0.0f;
        auto it = std::find(textures.begin(), textures.end(), textureID);
        if (it != textures.end()) {
            texID = static_cast<float>(it - textures.begin());
        }
        else {
            if (textures.size() >= MAX_TEXTURES) {
                End();
                Render(currentShader, *currentCamera);
                Begin();
            }
            textures.push_back(textureID);
            texID = static_cast<float>(textures.size() - 1);
        }

        // حساب UV حسب موقع الإطار
        float spriteW = 1.0f / sheetCols;
        float spriteH = 1.0f / sheetRows;

        float u0 = frameX * spriteW;
        float v0 = frameY * spriteH;
        float u1 = u0 + spriteW;
        float v1 = v0 + spriteH;

        // إذا كان التكسشر مقلوب الـ v
        v0 = 1.0f - v0 - spriteH;
        v1 = v0 + spriteH;

        static constexpr float quad[6][2] = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
            {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
        };

        // UV لكل vertex بناءً على الإطار
        float texCoords[6][2] = {
            {u0, v0}, {u1, v0}, {u1, v1},
            {u0, v0}, {u0, v1}, {u1, v1}
        };

        // مركز الدوران
        float halfW = scale.x * 0.5f;
        float halfH = scale.y * 0.5f;
        float centerX = position.x + halfW;
        float centerY = position.y + halfH;

        float s = sinf(zMath::Math::radians(rotation));
        float c = cosf(zMath::Math::radians(rotation));

        for (int i = 0; i < 6; ++i) {
            // إحداثيات محلية بالنسبة للمركز
            float localX = quad[i][0] * scale.x - halfW;
            float localY = quad[i][1] * scale.y - halfH;

            // دوران
            float rx = localX * c - localY * s;
            float ry = localX * s + localY * c;

            // إحداثيات نهائية
            float finalX = centerX + rx;
            float finalY = centerY + ry;

            // إضافة البيانات للبفر
            sprites.push_back(finalX);
            sprites.push_back(finalY);

            sprites.push_back(texCoords[i][0]);
            sprites.push_back(texCoords[i][1]);

            sprites.push_back(color.r);
            sprites.push_back(color.g);
            sprites.push_back(color.b);
            sprites.push_back(color.a);

            sprites.push_back(texID);
        }
	}
	void SpritesRender::Render(Shader* shader, Camera2D& camera)
	{
		currentShader = shader;
		currentCamera = &camera;

		shader->Bind();

		shader->SetUniformMatrix4x4("projection", camera.GetProjection());
		shader->SetUniformMatrix4x4("view", camera.GetView());

		for (size_t i = 0; i < textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}

		int samplers[MAX_TEXTURES];
		for (int i = 0; i < MAX_TEXTURES; ++i) samplers[i] = i;
		shader->SetUniformIntArray("u_Textures", MAX_TEXTURES, samplers);

		VAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sprites.size() / 9));
		VAO.Unbind();

		shader->Unbind();
	}
}
