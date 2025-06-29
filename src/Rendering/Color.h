#pragma once
#include <sstream>

namespace EWorld
{
	struct Color
	{
		float r, g, b, a;
	};

	class Color2
	{
	public:
		float r, g, b, a;

		Color2(float r, float g, float b, float a) : r{ r }, g{ g }, b{ b }, a{ a }
		{
		}
		~Color2() = default;

		static Color2 Convert255(float r, float g, float b, float a)
		{
			return Color2{ r * 255, g * 255, b * 255, a * 255 };
		}
		static Color2 Convert1(float r, float g, float b, float a)
		{
			return Color2{ r / 255, g / 255, b / 255, a / 255 };;
		}

		std::string ToString()
		{
			std::stringstream ss;

			ss << "{ r(" << r << "), g(" << g << "), b(" << b << "), a(" << a << ") }";

			return ss.str();
		}

	public:
		static Color2 WHITE()
		{
			return Color2{ 255, 255, 255, 255 };;
		}
		static Color2 BLACK()
		{
			return Color2{ 0, 0, 0, 0 };;
		}
		static Color2 RED()
		{
			return Color2{ 255, 0, 0, 255 };;
		}

	};
}