#pragma once

#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Essentials/Color.h"
#include "../Essentials/Shader.h"
#include "../Essentials/Font.h"
#include "Camera2D.h"

namespace zRender
{
    class TextsRender
    {
    private:
        VertexArray VAO;
        VertexBuffer VBO;
        std::vector<float> texts;
        std::vector<uint32_t> fonts;
        Shader* currentShader;
        Camera2D* currentCamera;

    public:
        TextsRender();
        ~TextsRender();

        void Begin();
        void End();

        void DrawText(Font* font, const std::string& text, const zMath::Vector2& position, const zMath::Vector2& scale, float size, float padding, float wrap, Color color = WHITE);

        void Render(Shader* shader, Camera2D& camera);

    };
}