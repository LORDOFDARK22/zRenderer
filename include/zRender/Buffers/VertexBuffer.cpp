#include "VertexBuffer.h"
#include <glad/glad.h>

namespace zRender
{
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &ID);
	}
	void VertexBuffer::Data(std::vector<float>& vertices, bool dynamic)
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		if (dynamic)
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	}
	void VertexBuffer::Data(std::vector<Vertex>& vertices, bool dynamic)
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		if (dynamic)
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}
	void VertexBuffer::Data(const void* data, size_t size, bool dynamic)
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		if (dynamic)
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	void VertexBuffer::SubData(size_t offset, const void* data, size_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}
	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::Delete()
	{
		glDeleteBuffers(1, &ID);
	}
}