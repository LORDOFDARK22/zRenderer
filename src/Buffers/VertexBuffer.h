#pragma once
#include <vector>
#include <iostream>
#include "../Essentials/Vertex.h"

namespace zRender
{
	class VertexBuffer
	{
	private:
		uint32_t ID;

	public:
		VertexBuffer();

		void Data(std::vector<float>& vertices, bool dynamic);
		void Data(std::vector<Vertex>& vertices, bool dynamic);
		void Data(const void* data, size_t size, bool dynamic);

		void SubData(size_t offset, const void* data, size_t size);

		const uint32_t GetID() const { return ID; }

		void Bind();
		void Unbind();
		void Delete();
	};
}