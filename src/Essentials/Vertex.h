#include <zMath/Vector2.h>
#include <zMath/Vector3.h>

#include "../Essentials/Color.h"

namespace zRender
{
	struct Vertex
	{
		zMath::Vector3 position;
		zMath::Vector3 normal;
		zMath::Vector2 uv;
		Color color;
	};
}