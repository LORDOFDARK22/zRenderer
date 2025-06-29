#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>

namespace EWorld
{
	class VertexArray
	{
	private:
		uint32_t id;
		std::vector<uint32_t> buffers;

	public:
		VertexArray();
		~VertexArray();

		void GenBuffer(uint32_t layoutLocation, uint32_t numComponents, GLenum typeClass, GLsizeiptr sizeComponent, void* offset, GLboolean normalized, 
			int listCount, const void* data, GLenum typeDraw);

		void UpdateBuffers();

	};
}