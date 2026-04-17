#include <zMath/Vector2.h>

#include "../Essentials/Color.h"

namespace zRender
{
	struct Vertex
	{
		zMath::Vector2 position;
		zMath::Vector2 uv;
		Color color;
	};
}