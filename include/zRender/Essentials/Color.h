#pragma once
#include <zMath/Vector4.h>
#include <zMath/Random.h>
#include <format>

#include <ostream>

#define WHITE zRender::Color{ 1.0f, 1.0f, 1.0f, 1.0f}
#define SILVER zRender::Color{ 0.7f, 0.7f, 0.7f, 1.0f}
#define GRAY zRender::Color{ 0.4f, 0.4f, 0.4f, 1.0f}
#define BLACK zRender::Color{ 0.0f, 0.0f, 0.0f, 1.0f}

#define RED zRender::Color{ 1.0f, 0.0f, 0.0f, 1.0f}
#define GREEN zRender::Color{ 0.0f, 1.0f, 0.0f, 1.0f}
#define BLUE zRender::Color{ 0.0f, 0.0f, 1.0f, 1.0f}

#define ORANGE zRender::Color{ 1.0f, 0.5f, 0.0f, 1.0f}
#define YELLOW zRender::Color{ 1.0f, 1.0f, 0.0f, 1.0f}
#define LIME zRender::Color{ 0.0f, 1.0f, 0.5f, 1.0f}
#define SKYBLUE zRender::Color{ 0.0f, 1.0f, 1.0f, 1.0f}
#define PURPLE zRender::Color{ 0.5f, 0.0f, 1.0f, 1.0f}
#define PINK zRender::Color{ 1.0f, 0.0f, 1.0f, 1.0f}
#define MAGENTA zRender::Color{ 1.0f, 0.0f, 0.5f, 1.0f}

#define RandomColor zRender::Color{ zMath::Random::RandomRange(0.0f, 1.0f), zMath::Random::RandomRange(0.0f, 1.0f), zMath::Random::RandomRange(0.0f, 1.0f), 1.0f}

namespace zRender
{
	struct Color
	{
		//uint8_t
		float r, g, b, a = 1.0f;

		zMath::Vector4 ToVector()
		{
			return zMath::Vector4{ r, g, b, a };
		}
		Color To255()
		{
			return Color{ r * 255, g * 255, b * 255, a * 255 };
		}
		Color To1()
		{
			return Color{ r / 255, g / 255, b / 255, a / 255 };
		}

		uint32_t ToBit()
		{
			return
				(uint32_t)r |
				((uint32_t)g << 8) |
				((uint32_t)b << 16) |
				((uint32_t)a << 24);
		}

		static Color Convert255(float r, float g, float b, float a)
		{
			return Color{ r * 255, g * 255, b * 255, a * 255 };
		}
		static Color Convert1(float r, float g, float b, float a)
		{
			return Color{ r / 255, g / 255, b / 255, a / 255 };
		}

		static Color ColorVector(const zMath::Vector4& v)
		{
			return Color{ v.x, v.y, v.z, v.w };;
		}
		static zMath::Vector4 ColorVector(Color color)
		{
			return zMath::Vector4{ color.r, color.g, color.b, color.a };;
		}

		static uint32_t ColorBit(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			return
				(uint32_t)r |
				((uint32_t)g << 8) |
				((uint32_t)b << 16) |
				((uint32_t)a << 24);
		}
	};

	static Color operator*(Color a, Color b)
	{
		return Color(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);
	}
	static Color operator*(Color a, float s)
	{
		return Color(a.r * s, a.g * s, a.b * s, a.a * s);
	}
	static Color operator/(Color a, float d)
	{
		return Color(a.r / d, a.g / d, a.b / d, a.a / d);
	}
	static Color operator/(Color a, Color b)
	{
		return Color(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);
	}

	static std::ostream& operator<<(std::ostream& ostream, Color color)
	{
		ostream << "{ r" << color.r << ", g" << color.g << ", b" << color.b << ", a" << color.a << " }";
		return ostream;
	}

	struct UColor
	{
		uint8_t r, g, b, a;
	};
}

template<>
struct std::formatter<zRender::Color> : std::formatter<std::string>
{
	auto format(const zRender::Color& c, std::format_context& ctx) const
	{
		return std::formatter<std::string>::format(std::format("( r{}, g{}, b{}, a{} )", c.r, c.g, c.b, c.a), ctx);
	}
};