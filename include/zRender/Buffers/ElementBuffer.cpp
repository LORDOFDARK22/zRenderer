#include "ElementBuffer.h"
#include <glad/glad.h>

namespace zRender
{
	void ElementBuffer::Delete()
	{
		glDeleteBuffers(1, &ID);
	}
	ElementBuffer::ElementBuffer()
	{
		glGenBuffers(1, &ID);
	}
	void ElementBuffer::Data(std::vector<uint32_t>& indices, bool dynamic)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		if (dynamic)
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
		else
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	}
	void ElementBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}
	void ElementBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
