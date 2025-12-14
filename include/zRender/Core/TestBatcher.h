#pragma once
#include "Batcher.h"

namespace zRender
{
	class TestBatcher : public Batcher<Vertex, uint32_t>
	{
	public:
		TestBatcher();
		~TestBatcher() = default;

		void End() override;
		void Render(Shader* shader, Camera2D& camera) override;

	};
}