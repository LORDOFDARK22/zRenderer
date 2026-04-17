#include "UnicodeFont.h"

#include <fstream>
#include <vector>

#include <glad/glad.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "../../stb_truetype.h"

namespace zRender
{
    bool UnicodeFont::Load(const std::string& path, float size)
    {
        // -------------------------------
        // تحميل ملف الخط
        // -------------------------------
        std::ifstream file(path, std::ios::binary);
        if (!file) return false;

        file.seekg(0, std::ios::end);
        size_t length = file.tellg();
        file.seekg(0);

        std::vector<unsigned char> buffer(length);
        file.read(reinterpret_cast<char*>(buffer.data()), length);
        file.close();

        // -------------------------------
        // تهيئة font info
        // -------------------------------
        stbtt_fontinfo fontInfo;
        if (!stbtt_InitFont(&fontInfo, buffer.data(), 0))
            return false;

        float scale = stbtt_ScaleForPixelHeight(&fontInfo, size);

        // -------------------------------
        // إنشاء Atlas Bitmap
        // -------------------------------
        std::vector<unsigned char> bitmap(atlasWidth * atlasHeight);
        stbtt_pack_context pc;

        if (!stbtt_PackBegin(&pc, bitmap.data(), atlasWidth, atlasHeight, 0, 1, nullptr))
            return false;

        stbtt_PackSetOversampling(&pc, 2, 2);

        // -------------------------------
        // نطاق اللغة العربية
        // -------------------------------
        const uint32_t arabicRangeStart = 0x0600;
        const uint32_t arabicCount = 256;

        std::vector<stbtt_packedchar> packed(arabicCount);

        if (!stbtt_PackFontRange(&pc, buffer.data(), 0, size, arabicRangeStart, arabicCount, packed.data()))
        {
            stbtt_PackEnd(&pc);
            return false;
        }

        stbtt_PackEnd(&pc);

        // -------------------------------
        // إنشاء Texture OpenGL
        // -------------------------------
        if (ID == 0) glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // -------------------------------
        // تخزين الـ Range
        // -------------------------------
        rangeStart = arabicRangeStart;
        rangeCount = arabicCount;

        glyphs.clear();
        glyphs.resize(rangeCount);

        // -------------------------------
        // تحويل packed → Glyph مع bbMin / bbMax
        // -------------------------------
        for (uint32_t i = 0; i < arabicCount; i++)
        {
            const auto& p = packed[i];
            Glyph g;

            // UV في الـ Atlas
            g.uvMin = { p.x0 / (float)atlasWidth, p.y0 / (float)atlasHeight };
            g.uvMax = { p.x1 / (float)atlasWidth, p.y1 / (float)atlasHeight };

            // الحجم والإزاحة
            g.size = { (float)(p.x1 - p.x0), (float)(p.y1 - p.y0) };
            g.bearing = { (float)p.xoff, (float)p.yoff };
            g.advance = p.xadvance;

            // bounding box بالنسبة للـ baseline
            int x0, y0, x1, y1;
            stbtt_GetCodepointBitmapBox(&fontInfo, arabicRangeStart + i, scale, scale, &x0, &y0, &x1, &y1);

            g.bbMin = { (float)x0, (float)y0 };
            g.bbMax = { (float)x1, (float)y1 };

            glyphs[i] = g;
        }

        // -------------------------------
        // Fallback Glyph
        // -------------------------------
        fallbackGlyph.size = { size * 0.5f, size * 0.5f };
        fallbackGlyph.bearing = { 0.0f, 0.0f };
        fallbackGlyph.advance = size * 0.5f;
        fallbackGlyph.uvMin = { 0.0f, 0.0f };
        fallbackGlyph.uvMax = { 0.0f, 0.0f };
        fallbackGlyph.bbMin = { 0.0f, 0.0f };
        fallbackGlyph.bbMax = { size * 0.5f, size * 0.5f };

        return true;
    }

    const Glyph& UnicodeFont::GetGlyph(uint32_t codepoint) const
    {
        if (codepoint < rangeStart || codepoint >= rangeStart + rangeCount)
            return fallbackGlyph;

        return glyphs[codepoint - rangeStart];
    }


} // namespace zRender
