#pragma once
#include <iostream>
#include <zMath/Vector2.h>
#include <vector>

namespace zRender
{
    struct Glyph
    {
        zMath::Vector2 uvMin;
        zMath::Vector2 uvMax;

        zMath::Vector2 size;
        zMath::Vector2 bearing;

        zMath::Vector2 bbMin;
        zMath::Vector2 bbMax;

        float advance = 0.0f;
    };

    class UnicodeFont
    {
    public:
        bool Load(const std::string& path, float size);

        const Glyph& GetGlyph(uint32_t codepoint) const;

        inline uint32_t GetID() const { return ID; }

    private:
        uint32_t ID = 0;

        int atlasWidth = 1024;
        int atlasHeight = 1024;

        // 🔥 نطاق التخزين
        uint32_t rangeStart = 0;
        uint32_t rangeCount = 0;

        std::vector<Glyph> glyphs;

        // 🔥 fallback عشان ما يصير crash
        Glyph fallbackGlyph{};
    };
}
