#pragma once
#include <iostream>
#include <glad/glad.h>

#include "VertexBuffer.h"

namespace zRender
{
	class VertexArray
	{
	private:
		uint32_t ID;

	public:
		VertexArray();

		void LinkAttrib(VertexBuffer& vertexBuffer, uint32_t layout, uint32_t numComponents, GLenum type, size_t stride, void* offset);

		const uint32_t GetID() const { return ID; }

		void Bind();
		void Unbind();
		void Delete();
	};
}