#include "Font.h"

#include <fstream>
#include <vector>

#include <glad/glad.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "../../stb_truetype.h"

namespace zRender
{
    Font::Font(const std::string& fontPath, int width, int height, float size) : fontPath{ fontPath }, width { width }, height{ height }, size{ size }
	{
        std::ifstream fontStream{ fontPath, std::ios::binary };

        if (fontStream.fail())
        {
            std::cout << "Failed to load Font : " << fontPath << std::endl;
            return;
        }

        fontStream.seekg(0, fontStream.end);
        int64_t length = fontStream.tellg();
        fontStream.seekg(0, fontStream.beg);

        std::vector<unsigned char> buffer;
        buffer.resize(length);
        std::vector<unsigned char> bitmap;
        bitmap.resize(width * height);
        fontStream.read((char*)(&buffer[0]), length);

        data = (void*) new stbtt_bakedchar[96];

        stbtt_BakeFontBitmap(buffer.data(), 0, size, bitmap.data(), width, height, 32, 96, (stbtt_bakedchar*)(data));

        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	Font::~Font()
	{
        if (ID != 0)
            glDeleteTextures(1, &ID);

        if (data)
            delete data;
	}
    FontGlyph Font::GetGlyph(char c, zMath::Vector2& position)
    {
        FontGlyph glyph{};

        if (c >= 32 && c < 128)
        {
            stbtt_aligned_quad aq;
            stbtt_GetBakedQuad((stbtt_bakedchar*)(data), width, height, c - 32, &position.x, &position.y, &aq, 1);

            glyph =
            {
                .bbmin = {aq.x0, aq.y0},
                .bbmax = {aq.x1, aq.y1},

                .uvmin = {aq.s0, aq.t0},
                .uvmax = {aq.s1, aq.t1}
            };
        }

        return glyph;
    }
}
