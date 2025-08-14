#pragma once
#include <zMath/Vector4.h>

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

namespace zRender
{
	struct Color
	{
		float r, g, b, a = 1.0f;

		static Color Convert255(float r, float g, float b, float a)
		{
			return Color{ r * 255, g * 255, b * 255, a * 255 };
		}
		static Color Convert1(float r, float g, float b, float a)
		{
			return Color{ r / 255, g / 255, b / 255, a / 255 };;
		}

		static Color ColorVector(const zMath::Vector4& v)
		{
			return Color{ v.x, v.y, v.z, v.w };;
		}
		static zMath::Vector4 ColorVector(Color color)
		{
			return zMath::Vector4{ color.r, color.g, color.b, color.a };;
		}

	};
}